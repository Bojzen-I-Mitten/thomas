#pragma once
#pragma unmanaged
#include <thomas\graphics\Material.h>
#pragma managed
#include "Shader.h"
#include "math/Math.h"
#include "Utility.h"
using namespace System;
namespace ThomasEditor
{
	public ref class Material
	{
	internal:
		thomas::graphics::Material* nativePtr;
		Material(thomas::graphics::Material* ptr) { nativePtr = ptr; }
	public:
		Material(String^ name, Shader^ shader)
		{
			nativePtr = new thomas::graphics::Material(Utility::ConvertString(name), shader->nativePtr);
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
		property Shader^ Shader
		{
			ThomasEditor::Shader^ get() {return %ThomasEditor::Shader(nativePtr->GetShader()); }
			void set(ThomasEditor::Shader^ value) { nativePtr->SetShader(value->nativePtr); }
		}


	/*	Texture* GetTexture(String^ name);
		void SetTexture(String^ name, Texture& value);*/



	private:

	};
}