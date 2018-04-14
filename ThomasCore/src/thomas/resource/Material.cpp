#include "Material.h"
#include "Shader.h"
#include "../resource/texture/Texture.h"
#include "../graphics/Mesh.h"
#include "ShaderProperty\shaderProperties.h"
#include <mutex>
namespace thomas
{
	namespace resource
	{
		Material* Material::s_standardMaterial;
		UINT Material::s_idCounter = 0;

		void Material::FetchPropertiesFromShader()
		{
			if (m_shader)
			{
				for (auto& prop : m_shader->GetProperties())
				{
					if (prop.second->isMaterialProperty)
					{
						m_properties[prop.first] = prop.second;
					}
				}
			}

		}

		void Material::OnChanged()
		{
		}

		void Material::Lock()
		{
			((std::mutex*)m_lock)->lock();
		}

		void Material::Unlock()
		{
			((std::mutex*)m_lock)->unlock();
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
			m_lock = new std::mutex();
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
			FetchPropertiesFromShader();

		}


		Material::Material(Material * original) : Resource("")
		{
			m_lock = new std::mutex();
			m_baseMaterial = original->m_baseMaterial;
			m_isInstance = true;
			m_shader = original->m_shader;
			m_renderQueue = original->m_renderQueue;
			m_topology = original->m_topology;
			m_passes = original->m_passes;

			m_id = s_idCounter;
			s_idCounter++;

			FetchPropertiesFromShader();
		}

		Material::Material(std::string path) : Resource(path)
		{
			m_lock = new std::mutex();
			m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			m_shader = nullptr;
			m_isInstance = false;
			m_id = s_idCounter;
			s_idCounter++;
		}

		Material::~Material()
		{
			Lock();
			m_properties.clear();
			Unlock();
		}

		void Material::SetShader(resource::Shader * shader)
		{
			Lock();
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
			FetchPropertiesFromShader();
			Unlock();
		}

		resource::Shader * Material::GetShader()
		{
			return m_shader;
		}

		void Material::ApplyProperty(const std::string & name)
		{
			if (HasProperty(name))
			{
				Lock();
				m_properties[name]->Apply(m_shader);
				Unlock();
			}
		}
		

		bool Material::HasProperty(const std::string & name)
		{
			return m_shader->HasProperty(name);
		}

		std::shared_ptr<shaderProperty::ShaderProperty> Material::GetProperty(const std::string & name)
		{
			Lock();
			if (HasProperty(name))
			{
				return m_properties[name];
			}
			Unlock();
			return nullptr;
		}
		math::Color Material::GetColor(const std::string& name)
		{
			if (HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::VECTOR)
			{
				
				return ((shaderProperty::ShaderPropertyVector*)m_properties[name].get())->GetValue();
				
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
				return math::Color();
			}
		}
		void Material::SetColor(const std::string& name, const math::Color& value)
		{
	
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyVector(value));
			m_properties[name]->SetName(name);
			Unlock();

		}
		float Material::GetFloat(const std::string& name)
		{
			if (HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::SCALAR_FLOAT)
			{
				return ((shaderProperty::ShaderPropertyScalarFloat*)m_properties[name].get())->GetValue();
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
				return 0;
			}
		}
		void Material::SetFloat(const std::string& name, float& value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyScalarFloat(value));
			m_properties[name]->SetName(name);
			Unlock();
			
		}
		int Material::GetInt(const std::string& name)
		{
			if (HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::SCALAR_INT)
			{
				return ((shaderProperty::ShaderPropertyScalarInt*)m_properties[name].get())->GetValue();
			}
			else
			{
				//LOG("Property " << name << " does not exist for material" );
				return 0;
			}
		}
		void Material::SetInt(const std::string& name, int& value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyScalarInt(value));
			m_properties[name]->SetName(name);
			Unlock();
			
		}
		math::Matrix Material::GetMatrix(const std::string& name)
		{
			if (HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::MATRIX)
			{
				((shaderProperty::ShaderPropertyMatrix*)m_properties[name].get())->GetValue();
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
				return math::Matrix();
			}
		}
		void Material::SetMatrix(const std::string& name, math::Matrix& value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyMatrix(value));
			m_properties[name]->SetName(name);
			Unlock();
			
		}
		resource::Texture2D * Material::GetTexture2D(const std::string & name)
		{
			if (HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::TEXTURE2D)
			{
				return ((shaderProperty::ShaderPropertyTexture2D*)m_properties[name].get())->GetValue();
			}
			else
			{
				return nullptr;
			}
		}
		void Material::SetTexture2D(const std::string & name, resource::Texture2D* value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyTexture2D(value));
			m_properties[name]->SetName(name);
			Unlock();
		}

		math::Vector4 Material::GetVector(const std::string& name)
		{
			if(HasProperty(name) && m_properties[name]->GetType() == shaderProperty::ShaderProperty::Type::VECTOR)
			{
				return ((shaderProperty::ShaderPropertyVector*)m_properties[name].get())->GetValue();
			}
			else
			{
				//LOG("Property " << name << " does not exist for material");
				return math::Vector4();
			}
		}
		void Material::SetVector(const std::string& name, math::Vector4& value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyVector(value));
			m_properties[name]->SetName(name);
			Unlock();
			
		}
		void Material::SetResource(const std::string & name, ID3D11ShaderResourceView* value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyShaderResource(value));
			m_properties[name]->SetName(name);
			Unlock();
		}
		void Material::SetConstantBuffer(const std::string & name, ID3D11Buffer* value)
		{
			Lock();
			m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyConstantBuffer(value));
			m_properties[name]->SetName(name);
			Unlock();
		}


		void Material::SetShaderPassEnabled(int index, bool enabled)
		{
			Lock();
			if (m_passes.size() > index)
				m_passes[index].enabled = enabled;
			Unlock();
		}
		void Material::SetShaderPassEnabled(std::string name, bool enabled)
		{
			Lock();
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (m_passes[i].name == name)
				{
					m_passes[i].enabled = enabled;
					break;
				}

			}
			Unlock();
		}

		void Material::SetShaderPass(int index)
		{
			Lock();
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (i == index)
					m_passes[i].enabled = true;
				else
					m_passes[i].enabled = false;
			}
			Unlock();
		}

		void Material::SetShaderPass(std::string name)
		{
			Lock();
			for (int i = 0; i < m_passes.size(); i++)
			{
				if (m_passes[i].name == name)
					m_passes[i].enabled = true;
				else
					m_passes[i].enabled = false;

			}
			Unlock();
		}

		void Material::Bind()
		{
			Lock();
			m_shader->Bind();
			for (auto prop : m_properties)
			{
				prop.second->Apply(m_shader);
			}
			ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(m_topology);
			Unlock();
		}
		void Material::Draw(graphics::Mesh * mesh)
		{
			Lock();
			for (Pass p : m_passes)
			{
				if (p.enabled)
				{
					m_shader->SetPass(p.index);
					mesh->Draw(m_shader);
				}
			}
			Unlock();
		}
		void Material::Draw(UINT vertexCount, UINT startVertexLocation)
		{
			Lock();
			for (Pass p : m_passes)
			{
				if (p.enabled)
				{
					m_shader->SetPass(p.index);
					ThomasCore::GetDeviceContext()->Draw(vertexCount, startVertexLocation);
				}
			}
			Unlock();
		}
		std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> Material::GetEditorProperties()
		{
			Lock();
			std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> editorProperties;
			for (auto& prop : m_properties)
			{
				if (prop.second->isMaterialProperty)
				{
					editorProperties[prop.first] = prop.second;
				}
			}
			Unlock();
			return editorProperties;
		}
		std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> Material::GetAllProperties()
		{
			return m_properties;
		}
		UINT Material::GetId()
		{
			return m_id;
		}
	}
}