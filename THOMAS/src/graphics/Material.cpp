#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
namespace thomas
{
	namespace graphics
	{

		void Material::SetSampler(const std::string name, Texture & value)
		{
			if (m_shader->HasProperty(name))
			{
				GetProperty(name)->SetSampler(value);
			}
		}

		Material::Material(Shader * shader)
		{
			m_shader = shader;
		}

		void Material::SetShader(Shader * shader)
		{
			m_shader = shader;
			m_renderQueue = -1;
			m_properties.clear();
		}

		Shader * Material::GetShader()
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
				return nullptr;
			}
			else if (m_shader->HasProperty(name))
			{
				MaterialProperty* prop = new MaterialProperty(m_shader->GetProperty(name));
				m_properties.push_back(prop);
				return prop;
			}
			else
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
		void Material::SetColor(const std::string& name, math::Color& value)
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
		void Material::Bind()
		{
			for (auto prop : m_properties)
			{
				prop->ApplyProperty(m_shader);
			}
			m_shader->Bind();
		}
		void Material::Draw(Mesh * mesh)
		{
			Bind();
			mesh->Draw(m_shader);
		}
		void Material::Draw(UINT vertexCount, UINT startVertexLocation)
		{
			Bind();
			ThomasCore::GetDeviceContext()->Draw(vertexCount, startVertexLocation);
		}
		Material * Material::Find(std::string name)
		{
			return nullptr;
		}
	}
}