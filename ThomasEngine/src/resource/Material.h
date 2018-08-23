#pragma once
#pragma unmanaged
#include <thomas\resource\Material.h>
#include <thomas\resource\ShaderProperty\shaderProperties.h>
#pragma managed
#include "Shader.h"
#include "../math/Math.h"
#include "Resources.h"
#include "texture\Texture2D.h"
using namespace System::Collections::Generic;
namespace ThomasEngine
{
	[DataContractAttribute]
	public ref class Material : public Resource
	{
	private:
		Shader ^ m_shaderBeforePlay;
		Dictionary<String^, System::Object^>^ m_propertiesBeforePlay;
	internal:
		Material(thomas::resource::Material* ptr) : Resource(Utility::ConvertString(ptr->GetPath()), ptr) {};
		bool m_loaded = false;
	public:

		~Material()
		{
			delete m_nativePtr;
		}


		Material(Shader^ shader) : Resource(shader->name + " Material.mat", new thomas::resource::Material((thomas::resource::Shader*)shader->m_nativePtr))
		{
			m_loaded = true;
		}
		Material(Material^ original) : Resource(original->ToString() + " (instance).mat", new thomas::resource::Material((thomas::resource::Material*)original->m_nativePtr))
		{
			m_loaded = true;
		}


		void OnPlay() override
		{
			m_shaderBeforePlay = this->Shader;
			m_propertiesBeforePlay = this->EditorProperties;
		}

		void OnStop() override
		{
			this->Shader = m_shaderBeforePlay;
			this->EditorProperties = m_propertiesBeforePlay;
			m_shaderBeforePlay = nullptr;
			m_propertiesBeforePlay = nullptr;
		}

		property String^ name
		{
			String^ get() override { 
				if (m_path->Length > 0)
					return System::IO::Path::GetFileNameWithoutExtension(m_path);
				else
				{
					return "Default Material";
				}
			}
		};
		
		static property Material^ StandardMaterial
		{
			Material^ get() { return gcnew Material(thomas::resource::Material::GetStandardMaterial()); }
		}
		
		void SetShaderPassEnabled(int index, bool enabled) { ((thomas::resource::Material*)m_nativePtr)->SetShaderPassEnabled(index, enabled); }
		void SetShaderPassEnabled(std::string name, bool enabled) { ((thomas::resource::Material*)m_nativePtr)->SetShaderPassEnabled(name, enabled); }

		Color GetColor(String^ name) { return Color(((thomas::resource::Material*)m_nativePtr)->GetColor(Utility::ConvertString(name))); }
		void SetColor(String^ name, Color value) { ((thomas::resource::Material*)m_nativePtr)->SetColor(Utility::ConvertString(name), thomas::math::Color(value.r, value.g, value.b, value.a)); }

		float GetFloat(String^ name) { return ((thomas::resource::Material*)m_nativePtr)->GetFloat(Utility::ConvertString(name)); }
		void SetFloat(String^ name, float& value) { ((thomas::resource::Material*)m_nativePtr)->SetFloat(Utility::ConvertString(name), value); };

		int GetInt(String^ name) { return ((thomas::resource::Material*)m_nativePtr)->GetInt(Utility::ConvertString(name)); };
		void SetInt(String^ name, int& value) { ((thomas::resource::Material*)m_nativePtr)->SetInt(Utility::ConvertString(name),value); }

		Matrix4x4 GetMatrix(String^ name) { return Matrix4x4(((thomas::resource::Material*)m_nativePtr)->GetMatrix(Utility::ConvertString(name))); }
		void SetMatrix(String^ name, Matrix4x4 value) { ((thomas::resource::Material*)m_nativePtr)->SetMatrix(Utility::ConvertString(name), value.ToThomas()); }

		Vector4 GetVector(String^ name) { return Vector4(((thomas::resource::Material*)m_nativePtr)->GetVector(Utility::ConvertString(name))); }
		void SetVector(String^ name, Vector4 value) { ((thomas::resource::Material*)m_nativePtr)->SetVector(Utility::ConvertString(name), thomas::math::Vector4(value.x, value.y, value.z, value.w)); }

		Texture2D^ GetTexture2D(String^ name) 
		{
			thomas::resource::Texture2D* nativePtr = ((thomas::resource::Material*)m_nativePtr)->GetTexture2D(Utility::ConvertString(name));
			ThomasEngine::Resource^ texture = ThomasEngine::Resources::FindResourceFromNativePtr(nativePtr);
			if (texture)
				return (ThomasEngine::Texture2D^)texture;
			else
				return gcnew ThomasEngine::Texture2D(nativePtr);
		}
		void SetTexture2D(String^ name, Texture2D^ value) { ((thomas::resource::Material*)m_nativePtr)->SetTexture2D(Utility::ConvertString(name), (thomas::resource::Texture2D*)value->m_nativePtr); }
		
		[DataMemberAttribute(Order=0)]
		property Shader^ Shader
		{
			ThomasEngine::Shader^ get() {
				thomas::resource::Shader* nativePtr = ((thomas::resource::Material*)m_nativePtr)->GetShader();
				ThomasEngine::Resource^ shader = ThomasEngine::Resources::FindResourceFromNativePtr(nativePtr);
				if (shader)
					return (ThomasEngine::Shader^)shader;
				else
					return gcnew ThomasEngine::Shader(nativePtr);

			}
			void set(ThomasEngine::Shader^ value);
		}



		[DataMemberAttribute(Order = 1)]
		property Dictionary<String^, System::Object^>^ EditorProperties
		{
			Dictionary<String^, System::Object^>^ get() {
				return GetEditorProperties();
			}
				
			void set(Dictionary<String^, System::Object^>^ value);
		}
	private:
		Dictionary<String^, System::Object^>^ GetEditorProperties()
		{
			Dictionary<String^, System::Object^>^ properties = gcnew Dictionary<String^, System::Object^>();
			for (auto& prop : ((thomas::resource::Material*)m_nativePtr)->GetEditorProperties())
			{
				String^ name = Utility::ConvertString(prop.first);
				System::Object^ value;
				switch (prop.second->GetType())
				{
				case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_BOOL:

					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_FLOAT:
					value = GetFloat(name);
					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_INT:
					value = GetInt(name);
					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::VECTOR:
					value = GetVector(name);
					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::COLOR:
					value = GetColor(name);
					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::MATRIX:
					value = GetMatrix(name);
					break;
				case thomas::resource::shaderProperty::ShaderProperty::Type::TEXTURE2D:
					value = GetTexture2D(name);
					break;
				default:
					break;
				}
				properties->Add(name, value);
			}
			return properties;
		}

		

	internal:
		[OnDeserializedAttribute]
		void OnDeserialized(StreamingContext c)
		{
			m_loaded = true;
		}
				
	/*	Texture* GetTexture(String^ name);
		void SetTexture(String^ name, Texture& value);*/

	};
}