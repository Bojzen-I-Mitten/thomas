#pragma once
#include "../Common.h"
#include "Shader.h"
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

			PropClass GetPropClass(D3D_SHADER_VARIABLE_CLASS Class, D3D_SHADER_VARIABLE_TYPE type);
			PropType GetPropType(D3D_SHADER_VARIABLE_TYPE type);
			PropClass GetObjectPropClass(D3D_SHADER_VARIABLE_TYPE type);
			TexDim GetTextureDimension(D3D_SHADER_VARIABLE_TYPE type);
		public:
			MaterialProperty(Shader::ShaderVariable shaderVariable);
			void ApplyProperty(Shader* shader);

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

		private:

			bool m_isSet;
			unsigned int m_numberOfElements;
			std::string m_name;
			unsigned int m_index;
			void* m_value;
			PropClass m_class;
			PropType m_type;
			TexDim m_textureDimension;
			size_t m_rawSize;
			UINT m_rawCount;
		};


	}
}