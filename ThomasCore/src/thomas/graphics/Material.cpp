#include "Material.h"
#include "../resource/Shader.h"
#include "Texture.h"
#include "Mesh.h"
namespace thomas
{
	namespace graphics
	{
		std::vector<Material*> Material::s_materials;
		std::vector<Material*> Material::s_materialInstances;
		Material* Material::s_standardMaterial;

		UINT Material::s_idCounter = 0;

		void Material::SetSampler(const std::string name, Texture & value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetSampler(value);
			}
		}

		void Material::CreateProperties()
		{
			for (MaterialProperty* mProp : m_properties)
			{
				delete mProp;
			}
			m_properties.clear();

			if (m_shader)
			{
				for (graphics::MaterialProperty* prop : m_shader->GetProperties())
				{
					m_properties.push_back(new graphics::MaterialProperty(prop));
				}
			}
		}

		void Material::Init()
		{
			s_standardMaterial = new Material(resource::Shader::GetStandardShader());
		}

		void Material::Destroy()
		{
			for (Material* material : s_materialInstances)
			{
				delete material;
			}
			s_materialInstances.clear();
			s_materials.clear();
		}

		Material * Material::GetStandardMaterial()
		{
			return s_standardMaterial;
		}

		Material::Material(resource::Shader * shader)
		{
			m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_shader = shader;
			m_name = shader->GetName();
			s_materialInstances.push_back(this);
			s_materials.push_back(this);
			m_isInstance = false;
			m_id = s_idCounter;
			s_idCounter++;

			for (resource::Shader::ShaderPass p : *shader->GetPasses())
			{
				Pass pass;
				pass.name = p.name;
				pass.enabled = true;
				pass.index = m_passes.size();
				m_passes.push_back(pass);
			}

			CreateProperties();
		}

		Material::Material(std::string name, resource::Shader * shader) : Material(shader)
		{
			m_name = name;
		}

		Material::Material()
		{
			m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_shader = nullptr;
			s_materialInstances.push_back(this);
			s_materials.push_back(this);
			m_isInstance = false;
			m_id = s_idCounter;
			s_idCounter++;
		}

		Material::Material(Material * original)
		{
			m_baseMaterial = original->GetBaseMaterial();
			m_isInstance = true;
			m_shader = original->m_shader;
			m_renderQueue = original->m_renderQueue;
			m_name = original->m_name;
			m_topology = original->m_topology;
			m_passes = original->m_passes;
			for (MaterialProperty* prop : original->m_properties)
			{
				m_properties.push_back(new MaterialProperty(prop));
			}
			s_materialInstances.push_back(this);

			m_id = s_idCounter;
			s_idCounter++;
		}

		Material::~Material()
		{
			for (MaterialProperty* mProp : m_properties)
			{
				delete mProp;
			}
			m_properties.clear();
		}

		void Material::SetShader(resource::Shader * shader)
		{
			m_shader = shader;
			m_renderQueue = -1;
			m_passes.clear();
			for (resource::Shader::ShaderPass p : *m_shader->GetPasses())
			{
				Pass pass;
				pass.name = p.name;
				pass.enabled = true;
				pass.index = m_passes.size();
				m_passes.push_back(pass);
			}
			CreateProperties();
		}

		resource::Shader * Material::GetShader()
		{
			return m_shader;
		}

		std::string Material::GetName()
		{
			return m_name;
		}

		void Material::SetName(std::string name)
		{
			m_name = name;
		}

		bool Material::HasProperty(const std::string & name)
		{
			for (MaterialProperty* prop : m_properties)
			{
				if (prop->GetName() == name)
					return true;
			}
			return false;
		}

		MaterialProperty* Material::GetProperty(const std::string & name)
		{
			if (HasProperty(name))
			{
				for (MaterialProperty* prop : m_properties)
				{
					if (prop->GetName() == name)
						return prop;
				}
			}
			return nullptr;
		}
		math::Color* Material::GetColor(const std::string& name)
		{
			if (HasProperty(name))
			{
				return (math::Color*)GetProperty(name)->GetVector();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetColor(const std::string& name, const math::Color& value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetVector(value.ToVector4());
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		float* Material::GetFloat(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetFloat();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetFloat(const std::string& name, float& value)
		{
			
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetFloat(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}

		}
		int* Material::GetInt(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetInt();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetInt(const std::string& name, int& value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetInt(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		math::Matrix* Material::GetMatrix(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetMatrix();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetMatrix(const std::string& name, math::Matrix& value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetMatrix(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		Texture* Material::GetTexture(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetTexture();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetTexture(const std::string& name, Texture& value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetTexture(value);
				SetSampler("sampler" + name, value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		math::Vector4* Material::GetVector(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetVector();
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
				return nullptr;
			}
		}
		void Material::SetVector(const std::string& name, math::Vector4& value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetVector(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		void Material::SetResource(const std::string & name, ID3D11ShaderResourceView & value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetResource(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		void Material::SetBuffer(const std::string & name, ID3D11Buffer & value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetBuffer(value);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}
		void Material::SetRaw(const std::string & name, void * value, size_t size, UINT count)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetRaw(value, size, count);
			}
			else
			{
				LOG("Property " << name << " does not exist for material:" << m_name);
			}
		}


		void Material::SetShaderPassEnabled(int index, bool enabled)
		{
			if (m_passes.size() > index)
				m_passes[index].enabled = enabled;
		}
		void Material::SetShaderPassEnabled(std::string name, bool enabled)
		{
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (m_passes[i].name == name)
				{
					m_passes[i].enabled = enabled;
					break;
				}

			}
		}

		void Material::SetShaderPass(int index)
		{
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (i == index)
					m_passes[i].enabled = true;
				else
					m_passes[i].enabled = false;
			}
		}

		void Material::SetShaderPass(std::string name)
		{
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (m_passes[i].name == name)
					m_passes[i].enabled = true;
				else
					m_passes[i].enabled = false;

			}
		}

		void Material::Bind()
		{
			m_shader->Bind();
			for (auto prop : m_properties)
			{
				prop->ApplyProperty(m_shader);
			}
			ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(m_topology);
		}
		void Material::Draw(Mesh * mesh)
		{
			for (Pass p : m_passes)
			{
				if (p.enabled)
				{
					m_shader->SetPass(p.index);
					mesh->Draw(m_shader);
				}
			}
			
		}
		void Material::Draw(UINT vertexCount, UINT startVertexLocation)
		{
			for (Pass p : m_passes)
			{
				if (p.enabled)
				{
					m_shader->SetPass(p.index);
					ThomasCore::GetDeviceContext()->Draw(vertexCount, startVertexLocation);
				}
			}
		}
		std::vector<MaterialProperty*> Material::GetEditorProperties()
		{
			//optimize maybe?
			std::vector<MaterialProperty*> editorProperties;
			for (MaterialProperty* matProp : m_properties)
				if (matProp->GetBufferName() == "MATERIAL_PROPERTIES")
					editorProperties.push_back(matProp);
			return editorProperties;
		}
		std::vector<MaterialProperty*> Material::GetAllProperties()
		{
			return m_properties;
		}
		Material * Material::GetBaseMaterial()
		{
			if (m_isInstance)
				return m_baseMaterial->GetBaseMaterial();
			else
				return this;
		}
		Material * Material::Find(std::string name)
		{
			for (Material* mat : s_materials)
			{
				if (mat->m_name == name)
					return mat;
			}
			return nullptr;
		}
		std::vector<Material*>* Material::GetMaterialInstances()
		{
			return &s_materialInstances;
		}
		UINT Material::GetId()
		{
			return m_id;
		}
	}
}