#pragma once
#pragma unmanaged
#include <thomas\graphics\Material.h>
#pragma managed
#include "resource\Shader.h"
#include "math/Math.h"
using namespace System;
using namespace System::Collections::Generic;
namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Material
	{
	internal:
		thomas::graphics::Material* nativePtr;
		Material(thomas::graphics::Material* ptr) { nativePtr = ptr; }
	public:

		Material()
		{
			nativePtr = new thomas::graphics::Material();
		}

		Material(String^ name, Shader^ shader)
		{
			nativePtr = new thomas::graphics::Material(Utility::ConvertString(name), (thomas::resource::Shader*)shader->m_nativePtr);
		}
		Material(Material^ original)
		{
			nativePtr = new thomas::graphics::Material(original->nativePtr);
		}
		static Material^ Find(String^ name) {
			thomas::graphics::Material* m = thomas::graphics::Material::Find(Utility::ConvertString(name));
			if (m)
				return %Material(m);
			else
				return nullptr;
		};

		void SetShaderPassEnabled(int index, bool enabled) { nativePtr->SetShaderPassEnabled(index, enabled); }
		void SetShaderPassEnabled(std::string name, bool enabled) { nativePtr->SetShaderPassEnabled(name, enabled); }

		Color GetColor(String^ name) { return Color(*nativePtr->GetColor(Utility::ConvertString(name))); }
		void SetColor(String^ name, Color value) { nativePtr->SetColor(Utility::ConvertString(name), thomas::math::Color(value.r, value.g, value.b, value.a)); }

		float GetFloat(String^ name) { return *nativePtr->GetFloat(Utility::ConvertString(name)); }
		void SetFloat(String^ name, float& value) { nativePtr->SetFloat(Utility::ConvertString(name), value); };

		int GetInt(String^ name) { return *nativePtr->GetInt(Utility::ConvertString(name)); };
		void SetInt(String^ name, int& value) { nativePtr->SetInt(Utility::ConvertString(name),value); }

		Matrix4x4 GetMatrix(String^ name) { return Matrix4x4(*nativePtr->GetMatrix(Utility::ConvertString(name))); }
		void SetMatrix(String^ name, Matrix4x4 value) { nativePtr->SetMatrix(Utility::ConvertString(name), value.ToThomas()); }

		Vector4 GetVector(String^ name) { return Vector4(*nativePtr->GetVector(Utility::ConvertString(name))); }
		void SetVector(String^ name, Vector4 value) { nativePtr->SetVector(Utility::ConvertString(name), thomas::math::Vector4(value.x, value.y, value.z, value.w)); }

		Material^ GetBaseMaterial() { return %Material(nativePtr->GetBaseMaterial()); }

		property String^ Name {
			String^ get() { return Utility::ConvertString(nativePtr->GetName()); }
			void set(String^ value) { nativePtr->SetName(Utility::ConvertString(value)); }
		};
		[DataMemberAttribute]
		property Shader^ Shader
		{
			ThomasEditor::Shader^ get() {return %ThomasEditor::Shader(nativePtr->GetShader()); }
			void set(ThomasEditor::Shader^ value) { nativePtr->SetShader((thomas::resource::Shader*)value->m_nativePtr); }
		}
		
	internal:
		[DataMemberAttribute]
		property Dictionary<String^, System::Object^>^ EditorProperties
		{
			Dictionary<String^, System::Object^>^ get() {
				Dictionary<String^, System::Object^>^ props = gcnew Dictionary<String^, System::Object^>();
				for each(thomas::graphics::MaterialProperty* prop in nativePtr->GetEditorProperties())
				{

					String^ name = Utility::ConvertString(prop->GetName());
					Object^ obj;
					switch (prop->GetPropClass())
					{
					case thomas::graphics::MaterialProperty::PropClass::Scalar:
					{
						switch (prop->GetPropType())
						{
						case thomas::graphics::MaterialProperty::PropType::Bool:
						{
							obj = *prop->GetBool();
							break;
						}
						case thomas::graphics::MaterialProperty::PropType::Int:
						{
							obj = *prop->GetInt();
							break;
						}
						case thomas::graphics::MaterialProperty::PropType::Float:
						{
							obj = *prop->GetFloat();
							break;
						}
						default:
							break;
						}
						break;
					}
					case thomas::graphics::MaterialProperty::PropClass::Vector:
					{
						thomas::math::Vector4* v = prop->GetVector();
						obj = Vector4(*v);
						break;
					}
					case thomas::graphics::MaterialProperty::PropClass::Texture:
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

			}
		}
				
	/*	Texture* GetTexture(String^ name);
		void SetTexture(String^ name, Texture& value);*/

	private:

	};
}