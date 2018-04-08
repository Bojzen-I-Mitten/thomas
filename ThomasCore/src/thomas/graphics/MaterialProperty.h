#pragma once
#include "../Common.h"
#include "../resource/Shader.h"
#include "../utils/Math.h"
namespace thomas
{
	namespace graphics
	{
		class Texture;
		class THOMAS_API MaterialProperty
		{
		public:
			enum class PropClass
			{
				Unknown,
				Scalar,
				Vector,
				Texture,
				TextureSampler,
				Matrix,
				Buffer,
				Resource,
				UAV
			};

			enum class PropType
			{
				Void,
				Bool,
				Float,
				Int,
			};

			enum class TexDim
			{
				Unknown = -1,
				None = 0,
				Tex1D = 1,
				Tex2D = 2,
				Tex3D = 3,
				Cube = 4,
				Any = 6
			};
		private:
			void SetInitialData();
			PropClass GetPropClass(D3D_SHADER_VARIABLE_CLASS Class, D3D_SHADER_VARIABLE_TYPE type);
			PropType GetPropType(D3D_SHADER_VARIABLE_TYPE type);
			PropClass GetObjectPropClass(D3D_SHADER_VARIABLE_TYPE type);
			TexDim GetTextureDimension(D3D_SHADER_VARIABLE_TYPE type);
		public:
			MaterialProperty(UINT index, ID3DX11EffectVariable* variable);
			MaterialProperty(const MaterialProperty* otherProperty);
			~MaterialProperty();
			void ApplyProperty(resource::Shader* shader);

			std::string GetName();
			std::string GetBufferName();

			void SetBool(bool& value);
			void SetFloat(float& value);
			void SetInt(int& value);
			void SetVector(math::Vector4& value);
			void SetMatrix(math::Matrix& value);
			void SetTexture(Texture& value);
			void SetSampler(Texture& value);
			void SetResource(ID3D11ShaderResourceView& value);
			void SetBuffer(ID3D11Buffer& value);
			void SetUAV(ID3D11UnorderedAccessView& value);
			void SetRaw(void* value, size_t size, UINT count);
			void SetRaw(void* value);

			bool* GetBool();
			float* GetFloat();
			int* GetInt();
			math::Vector4* GetVector(); //TODO: Maybe not only vector4.
			math::Matrix* GetMatrix();
			Texture* GetTexture();
			Texture* GetSampler();

			ID3D11ShaderResourceView* GetResource();
			ID3D11Buffer* GetBuffer();
			ID3D11UnorderedAccessView* GetUAV();


			PropClass GetPropClass();
			PropType GetPropType();
			TexDim GetTexDim();

		private:
			bool m_isSet;

			unsigned int m_index;
			void* m_value;
			PropClass m_class;
			PropType m_type;
			TexDim m_textureDimension;
			size_t m_rawSize;
			UINT m_rawCount;
			D3DX11_EFFECT_TYPE_DESC m_typeDesc;
			D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;
			D3DX11_EFFECT_VARIABLE_DESC* m_bufferDesc;
		};


	}
}