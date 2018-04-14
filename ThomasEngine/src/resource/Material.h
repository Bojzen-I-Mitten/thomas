#pragma once
#pragma unmanaged
#include <thomas\resource\Material.h>
#include <thomas\resource\ShaderProperty\shaderProperties.h>
#pragma managed
#include "Shader.h"
#include "../math/Math.h"
#include "Resources.h"

using namespace System::Collections::Generic;
namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Material : public Resource
	{
	internal:
		Material(thomas::resource::Material* ptr) : Resource("", ptr) {};
		bool m_loaded = false;
	public:

		~Material()
		{
			delete m_nativePtr;
		}

		Material(Shader^ shader) : Resource("", new thomas::resource::Material((thomas::resource::Shader*)shader->m_nativePtr))
		{
			m_loaded = true;
		}
		Material(Material^ original) : Resource("", new thomas::resource::Material((thomas::resource::Material*)original->m_nativePtr))
		{
			m_loaded = true;
		}
		
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


		[DataMemberAttribute(Order=0)]
		property Shader^ Shader
		{
			ThomasEditor::Shader^ get() {return gcnew ThomasEditor::Shader(((thomas::resource::Material*)m_nativePtr)->GetShader()); }
			void set(ThomasEditor::Shader^ value) 
			{ 
				if(m_nativePtr)
					((thomas::resource::Material*)m_nativePtr)->SetShader((thomas::resource::Shader*)value->m_nativePtr);
				else
				{
					m_nativePtr = new thomas::resource::Material(Utility::ConvertString(m_path));
					((thomas::resource::Material*)m_nativePtr)->SetShader((thomas::resource::Shader*)value->m_nativePtr);
					
				}
			}
		}

		[DataMemberAttribute(Order = 1)]
		property Dictionary<String^, System::Object^>^ EditorProperties
		{
			Dictionary<String^, System::Object^>^ get() {
				Dictionary<String^, System::Object^>^ props = gcnew Dictionary<String^, System::Object^>();

				for (auto& prop : ((thomas::resource::Material*)m_nativePtr)->GetEditorProperties())
				{
					
					System::Object^ value;
					switch (prop.second->GetType())
					{
					case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_BOOL:
						
						break;
					case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_FLOAT:
						value = ((thomas::resource::Material*)m_nativePtr)->GetFloat(prop.first);
						break;
					case thomas::resource::shaderProperty::ShaderProperty::Type::SCALAR_INT:
						value = ((thomas::resource::Material*)m_nativePtr)->GetInt(prop.first);
						break;
					case thomas::resource::shaderProperty::ShaderProperty::Type::VECTOR:
						value = Vector4(((thomas::resource::Material*)m_nativePtr)->GetVector(prop.first));
						break;
					case thomas::resource::shaderProperty::ShaderProperty::Type::COLOR:
						value = Color(((thomas::resource::Material*)m_nativePtr)->GetColor(prop.first));
						break;
					case thomas::resource::shaderProperty::ShaderProperty::Type::MATRIX:
						//value = Matrix4x4(((thomas::resource::Material*)m_nativePtr)->GetMatrix(prop.first));
						break;
					default:
						break;
					}
					props->Add(Utility::ConvertString(prop.first), value);
				}
				return props;
			}
				
			void set(Dictionary<String^, System::Object^>^ value)
			{
				for each(String^ key in value->Keys)
				{
					System::Object^ prop = value[key];
					Type^ t = prop->GetType();
					if (t == Vector4::typeid)
					{
						Vector4 v = (Vector4)prop;
						SetVector(key, v);
					}
					else if (t == Color::typeid)
					{
						Color v = (Color)prop;
						SetColor(key, v);
					}
					else if (t == System::Single::typeid)
					{
						//SetRaw(key, &prop);
					}
				}
				if(m_loaded)
					ThomasEditor::Resources::SaveResource(this, m_path);
			}
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