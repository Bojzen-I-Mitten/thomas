#include "Material.h"
#include "Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/Mesh.h"
#include "ShaderProperty.h"
namespace thomas
{
	namespace resource
	{
		Material* Material::s_standardMaterial;
		UINT Material::s_idCounter = 0;

		void Material::SetSampler(const std::string name, graphics::Texture & value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetSampler(value);
			}
		}

		void Material::CreateProperties()
		{
			for (ShaderProperty* mProp : m_properties)
			{
				delete mProp;
			}
			m_properties.clear();

			if (m_shader)
			{
				for (ShaderProperty* prop : m_shader->GetProperties())
				{
					m_properties.push_back(new resource::ShaderProperty(prop));
				}
			}
		}

		void Material::OnChanged()
		{
		}

		void Material::Init()
		{
			s_standardMaterial = new Material(resource::Shader::GetStandardShader());
		}

		void Material::Destroy()
		{
			delete s_standardMaterial;
		}

		Material * Material::GetStandardMaterial()
		{
			return s_standardMaterial;
		}

		Material::Material(resource::Shader * shader) : Resource("")
		{
			m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_shader = shader;
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


		Material::Material(Material * original) : Resource("")
		{
			m_baseMaterial = original->m_baseMaterial;
			m_isInstance = true;
			m_shader = original->m_shader;
			m_renderQueue = original->m_renderQueue;
			m_topology = original->m_topology;
			m_passes = original->m_passes;
			for (ShaderProperty* prop : original->m_properties)
			{
				m_properties.push_back(new ShaderProperty(prop));
			}

			m_id = s_idCounter;
			s_idCounter++;
		}

		Material::Material(std::string path) : Resource(path)
		{
			m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_shader = nullptr;
			m_isInstance = false;
			m_id = s_idCounter;
			s_idCounter++;
		}

		Material::~Material()
		{
			for (ShaderProperty* mProp : m_properties)
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

		bool Material::HasProperty(const std::string & name)
		{
			for (ShaderProperty* prop : m_properties)
			{
				if (prop->GetName() == name)
					return true;
			}
			return false;
		}

		ShaderProperty* Material::GetProperty(const std::string & name)
		{
			if (HasProperty(name))
			{
				for (ShaderProperty* prop : m_properties)
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
				//LOG("Property " << name << " does not exist for material");
				return nullptr;
			}
		}
		void Material::SetColor(const std::string& name, const math::Color& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetVector(value.ToVector4());
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material" );
				return nullptr;
			}
		}
		void Material::SetInt(const std::string& name, int& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetInt(value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material");
				return nullptr;
			}
		}
		void Material::SetMatrix(const std::string& name, math::Matrix& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetMatrix(value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
			}
		}
		graphics::Texture* Material::GetTexture(const std::string& name)
		{
			if (HasProperty(name))
			{
				return GetProperty(name)->GetTexture();
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
				return nullptr;
			}
		}
		void Material::SetTexture(const std::string& name, graphics::Texture& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetTexture(value);
				SetSampler("sampler" + name, value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material");
				return nullptr;
			}
		}
		void Material::SetVector(const std::string& name, math::Vector4& value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetVector(value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
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
				//LOG("Property " << name << " does not exist for material");
			}
		}
		void Material::SetBuffer(const std::string & name, ID3D11Buffer & value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetBuffer(value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material" );
			}
		}
		void Material::SetRaw(const std::string & name, void * value, size_t size, UINT count)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetRaw(value, size, count);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
			}
		}

		void Material::SetRaw(const std::string & name, void * value)
		{
			if (HasProperty(name))
			{
				GetProperty(name)->SetRaw(value);
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
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
		void Material::Draw(graphics::Mesh * mesh)
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
		std::vector<ShaderProperty*> Material::GetEditorProperties()
		{
			//optimize maybe?
			std::vector<ShaderProperty*> editorProperties;
			for (ShaderProperty* matProp : m_properties)
				if (matProp->GetBufferName() == "MATERIAL_PROPERTIES")
					editorProperties.push_back(matProp);
			return editorProperties;
		}
		std::vector<ShaderProperty*> Material::GetAllProperties()
		{
			return m_properties;
		}
		UINT Material::GetId()
		{
			return m_id;
		}
	}
}