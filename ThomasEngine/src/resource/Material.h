#pragma once
#pragma unmanaged
#include <thomas\resource\Material.h>
#include <thomas\resource\ShaderProperty.h>
#pragma managed
#include "Shader.h"
#include "../math/Math.h"


using namespace System::Collections::Generic;
namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Material : public Resource
	{
	internal:
		Material(thomas::resource::Material* ptr) : Resource("", ptr) {};

		void SetRaw(String^ name, void* value) { ((thomas::resource::Material*)m_nativePtr)->SetRaw(Utility::ConvertString(name), value); }
	public:

		~Material()
		{
			delete m_nativePtr;
		}

		Material(Shader^ shader) : Resource("", new thomas::resource::Material((thomas::resource::Shader*)shader->m_nativePtr))
		{
		}
		Material(Material^ original) : Resource("", new thomas::resource::Material((thomas::resource::Material*)original->m_nativePtr))
		{
		}
		
		static property Material^ StandardMaterial
		{
			Material^ get() { return gcnew Material(thomas::resource::Material::GetStandardMaterial()); }
		}
		
		void SetShaderPassEnabled(int index, bool enabled) { ((thomas::resource::Material*)m_nativePtr)->SetShaderPassEnabled(index, enabled); }
		void SetShaderPassEnabled(std::string name, bool enabled) { ((thomas::resource::Material*)m_nativePtr)->SetShaderPassEnabled(name, enabled); }

		Color GetColor(String^ name) { return Color(*((thomas::resource::Material*)m_nativePtr)->GetColor(Utility::ConvertString(name))); }
		void SetColor(String^ name, Color value) { ((thomas::resource::Material*)m_nativePtr)->SetColor(Utility::ConvertString(name), thomas::math::Color(value.r, value.g, value.b, value.a)); }

		float GetFloat(String^ name) { return *((thomas::resource::Material*)m_nativePtr)->GetFloat(Utility::ConvertString(name)); }
		void SetFloat(String^ name, float& value) { ((thomas::resource::Material*)m_nativePtr)->SetFloat(Utility::ConvertString(name), value); };

		int GetInt(String^ name) { return *((thomas::resource::Material*)m_nativePtr)->GetInt(Utility::ConvertString(name)); };
		void SetInt(String^ name, int& value) { ((thomas::resource::Material*)m_nativePtr)->SetInt(Utility::ConvertString(name),value); }

		Matrix4x4 GetMatrix(String^ name) { return Matrix4x4(*((thomas::resource::Material*)m_nativePtr)->GetMatrix(Utility::ConvertString(name))); }
		void SetMatrix(String^ name, Matrix4x4 value) { ((thomas::resource::Material*)m_nativePtr)->SetMatrix(Utility::ConvertString(name), value.ToThomas()); }

		Vector4 GetVector(String^ name) { return Vector4(*((thomas::resource::Material*)m_nativePtr)->GetVector(Utility::ConvertString(name))); }
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
				for each(thomas::resource::ShaderProperty* prop in ((thomas::resource::Material*)m_nativePtr)->GetEditorProperties())
				{

					String^ name = Utility::ConvertString(prop->GetName());
					Object^ obj;
					switch (prop->GetPropClass())
					{
					case thomas::resource::ShaderProperty::PropClass::Scalar:
					{
						switch (prop->GetPropType())
						{
						case thomas::resource::ShaderProperty::PropType::Bool:
						{
							obj = *prop->GetBool();
							break;
						}
						case thomas::resource::ShaderProperty::PropType::Int:
						{
							obj = *prop->GetInt();
							break;
						}
						case thomas::resource::ShaderProperty::PropType::Float:
						{
							obj = *prop->GetFloat();
							break;
						}
						default:
							break;
						}
						break;
					}
					case thomas::resource::ShaderProperty::PropClass::Vector:
					{
						thomas::math::Vector4* v = prop->GetVector();
						obj = Vector4(*v);
						break;
					}
					case thomas::resource::ShaderProperty::PropClass::Texture:
					{
						break;
					}
					default:
						break;
					}

					if (obj != nullptr)
					{
						props->Add(name, obj);
					}
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
					else if (t == System::Single::typeid)
					{
						SetRaw(key, &prop);
					}
				}
			}
		}
	internal:
		
				
	/*	Texture* GetTexture(String^ name);
		void SetTexture(String^ name, Texture& value);*/

};
}