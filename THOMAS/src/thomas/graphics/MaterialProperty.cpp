#include "MaterialProperty.h"
#include "Texture.h"

namespace thomas
{
	namespace graphics
	{
		MaterialProperty::PropClass MaterialProperty::GetPropClass(D3D_SHADER_VARIABLE_CLASS Class, D3D_SHADER_VARIABLE_TYPE type)
		{
			PropClass tempClass;
			switch (Class)
			{
			case D3D_SVC_SCALAR:
				tempClass = PropClass::Scalar;
				break;
			case D3D_SVC_VECTOR:
				tempClass = PropClass::Vector;
				break;
			case D3D_SVC_MATRIX_COLUMNS:
			case D3D_SVC_MATRIX_ROWS:
				tempClass = PropClass::Matrix;
				break;
			case D3D_SVC_OBJECT:
				tempClass = GetObjectPropClass(type);
			default:
				break;
			}
			return tempClass;
		}

		MaterialProperty::PropType MaterialProperty::GetPropType(D3D_SHADER_VARIABLE_TYPE type)
		{
			PropType tempType;
			switch (type)
			{
			case D3D_SVT_BOOL:
				tempType = PropType::Bool;
				break;
			case D3D_SVT_INT:
			case D3D_SVT_UINT:
				tempType = PropType::Int;
				break;
			case D3D_SVT_FLOAT:
				tempType = PropType::Float;
				break;
			default:
				tempType = PropType::Void;
				break;
			}
			return tempType;
		}

		MaterialProperty::PropClass MaterialProperty::GetObjectPropClass(D3D_SHADER_VARIABLE_TYPE type)
		{
			PropClass tempClass;
			switch (type)
			{
			case D3D_SVT_TEXTURE:
			case D3D_SVT_TEXTURE1D:
			case D3D_SVT_TEXTURE2D:
			case D3D_SVT_TEXTURE3D:
			case D3D_SVT_TEXTURECUBE:
				tempClass = PropClass::Texture;
				break;
			case D3D_SVT_SAMPLER:
			case D3D_SVT_SAMPLER1D:
			case D3D_SVT_SAMPLER2D:
			case D3D_SVT_SAMPLER3D:
			case D3D_SVT_SAMPLERCUBE:
				tempClass = PropClass::TextureSampler;
				break;
			case D3D_SVT_BUFFER:
			case D3D_SVT_CBUFFER:
			case D3D_SVT_TBUFFER:
				tempClass = PropClass::Buffer;
				break;
			case D3D_SVT_STRUCTURED_BUFFER:
				tempClass = PropClass::Resource;
			case D3D_SVT_RWSTRUCTURED_BUFFER:
				tempClass = PropClass::UAV;
				break;
			default:
				tempClass = PropClass::Unknown;
				break;
			}
			return tempClass;
		}

		MaterialProperty::TexDim MaterialProperty::GetTextureDimension(D3D_SHADER_VARIABLE_TYPE type)
		{
			TexDim dimension;
			switch (type)
			{
			case D3D_SVT_TEXTURE:
				dimension = TexDim::Any;
				break;
			case D3D_SVT_TEXTURE1D:
				dimension = TexDim::Tex1D;
				break;
			case D3D_SVT_TEXTURE2D:
				dimension = TexDim::Tex2D;
				break;
			case D3D_SVT_TEXTURE3D:
				dimension = TexDim::Tex3D;
				break;
			case D3D_SVT_TEXTURECUBE:
				dimension = TexDim::Cube;
				break;
			default:
				dimension = TexDim::Unknown;
				break;
			}
			return dimension;
		}

		MaterialProperty::MaterialProperty(UINT index, ID3DX11EffectVariable* variable)
		{
			m_value = nullptr;
			m_isSet = false;
			m_index = index;
			variable->GetType()->GetDesc(&m_typeDesc);
			variable->GetDesc(&m_variableDesc);
			variable->GetParentConstantBuffer()->GetDesc(&m_bufferDesc);
			m_class = GetPropClass(m_typeDesc.Class, m_typeDesc.Type);

			switch (m_class)
			{
				case PropClass::Scalar:
				case PropClass::Vector:
				case PropClass::Matrix:
					m_type = GetPropType(m_typeDesc.Type);
					break;
				case PropClass::Texture:
					m_textureDimension = GetTextureDimension(m_typeDesc.Type);
					break;
				default:
					break;
			}
			
		}

			MaterialProperty::MaterialProperty(const MaterialProperty* otherProperty)
			{
				m_isSet = otherProperty->m_isSet;
				m_index = otherProperty->m_index;
				m_typeDesc = otherProperty->m_typeDesc;
				m_variableDesc = otherProperty->m_variableDesc;
				m_class = otherProperty->m_class;
				m_type = otherProperty->m_type;
				m_textureDimension = otherProperty->m_textureDimension;
				m_value = otherProperty->m_value;
				m_rawCount = otherProperty->m_rawCount;
				m_rawSize = otherProperty->m_rawSize;
				
			}

			MaterialProperty::~MaterialProperty()
			{
				switch (m_class)
				{
				case PropClass::Scalar:
				case PropClass::Vector:
				case PropClass::Matrix:
					SAFE_DELETE(m_value);
				default:
					break;
				}
			}

			void MaterialProperty::ApplyProperty(Shader * shader)
		{
			if (!m_isSet)
				return;
			ID3DX11EffectVariable* variable = shader->GetEffect()->GetVariableByIndex(m_index);
			if (!variable->IsValid())
			{
				LOG("Failed to Apply property to variable: " << GetName());
			}
			HRESULT result;
			switch (m_class)
			{
			case PropClass::Scalar:
			{
				ID3DX11EffectScalarVariable* scalar = variable->AsScalar();
				switch (m_type)
				{
				case PropType::Bool:
					result = scalar->SetBool(GetBool());
					break;
				case PropType::Float:
					result = scalar->SetFloat(*GetFloat());
					break;
				case PropType::Int:
					result = scalar->SetInt(*GetInt());
					break;
				default:
					break;
				}
				break;
			}
				
			case PropClass::Vector:
			{
				ID3DX11EffectVectorVariable* vector = variable->AsVector();
				switch (m_type)
				{
				case PropType::Float:
				{
					float* vectorPtr = ((DirectX::XMVECTOR*)GetVector())->vector4_f32;
					result = vector->SetFloatVector(vectorPtr);
					break;
				}
					
				default:
					break;
				}
				break;
			}
				
			case PropClass::Texture:
			case PropClass::Resource:
			{		
				ID3DX11EffectShaderResourceVariable* texture = variable->AsShaderResource();
				switch (m_class)
				{
				case PropClass::Texture: result = texture->SetResource(GetTexture()->GetTextureView()); break;
				case PropClass::Resource: result = texture->SetResource(GetResource()); break;
				default:
					break;
				}
				break;
			}
				
			case PropClass::TextureSampler:
			{
				ID3DX11EffectSamplerVariable* sampler = variable->AsSampler();
				result = sampler->SetSampler(0, GetSampler()->GetSamplerState());
				break;
			}
				
			case PropClass::Matrix:
			{
				ID3DX11EffectMatrixVariable* matrix = variable->AsMatrix();
				switch (m_type)
				{
				case PropType::Float:
					result = matrix->SetMatrix((float*)GetMatrix()->m);
					break;
				default:
					break;
				}
				break;
			}
			case PropClass::Buffer:
			{
				ID3DX11EffectConstantBuffer* buffer = variable->AsConstantBuffer();
				result = buffer->SetConstantBuffer(GetBuffer());
				break;
			}	
			case PropClass::Unknown:
				result = variable->SetRawValue(m_value, 0, m_rawSize*m_rawCount);
				break;
			default:
				break;
			}
			if (result != S_OK)
			{
				LOG("Failed to apply material property to shader: " << shader->GetName());
			}
		}

		std::string MaterialProperty::GetName()
		{
			return m_variableDesc.Name;
		}

		std::string MaterialProperty::GetBufferName()
		{
			return m_bufferDesc.Name;
		}

		void MaterialProperty::SetBool(bool & value)
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Bool)
			{
				SAFE_DELETE(m_value);
				m_value = new bool(value);
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: bool");
			}
		}

		void MaterialProperty::SetFloat(float & value)
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Float)
			{
				SAFE_DELETE(m_value);
				m_value = new float(value);
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: float");
			}
		}

		void MaterialProperty::SetInt(int & value)
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Int)
			{
				SAFE_DELETE(m_value);
				m_value = new int(value);
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: int");
			}
		}

		void MaterialProperty::SetVector(math::Vector4 & value)
		{
			if (m_class == PropClass::Vector)
			{
				SAFE_DELETE(m_value);
				m_value = new math::Vector4(value);
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: vector");
			}
		}

		void MaterialProperty::SetMatrix(math::Matrix & value)
		{
			if (m_class == PropClass::Matrix)
			{
				SAFE_DELETE(m_value);
				m_value = new math::Matrix(value);
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: matrix");
			}
		}

		void MaterialProperty::SetTexture(Texture & value)
		{
			if (m_class == PropClass::Texture)
			{
				m_value = (void*)&value;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: texture");
			}
		}

		void MaterialProperty::SetSampler(Texture & value)
		{
			if (m_class == PropClass::TextureSampler)
			{
				m_value = (void*)&value;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: texture sampler");
			}
		}

		void MaterialProperty::SetResource(ID3D11ShaderResourceView & value)
		{
			if (m_class == PropClass::Resource)
			{
				m_value = (void*)&value;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: resource");
			}
		}

		void MaterialProperty::SetBuffer(ID3D11Buffer & value)
		{
			if (m_class == PropClass::Buffer)
			{
				m_value = (void*)&value;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: buffer");
			}
		}

		void MaterialProperty::SetUAV(ID3D11UnorderedAccessView & value)
		{
			if (m_class == PropClass::UAV)
			{
				m_value = (void*)&value;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: Unordered access view");
			}
		}

		void MaterialProperty::SetRaw(void * value, size_t size, UINT count)
		{
			if (m_class == PropClass::Unknown)
			{
				m_value = value;
				m_rawSize = size;
				m_rawCount = count;
				m_isSet = true;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: unknown");
			}
		}



		bool* MaterialProperty::GetBool()
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Bool)
			{
				return (bool*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: bool");
				return nullptr;
			}
		}

		float* MaterialProperty::GetFloat()
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Float)
			{
				return (float*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: float");
				return nullptr;
			}
		}

		int* MaterialProperty::GetInt()
		{
			if (m_class == PropClass::Scalar && m_type == PropType::Int)
			{
				return (int*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: int");
				return nullptr;
			}
		}


		math::Vector4* MaterialProperty::GetVector()
		{
			if (m_class == PropClass::Vector)
			{
				return (math::Vector4*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: vector");
				return nullptr;
			}
		}

		math::Matrix* MaterialProperty::GetMatrix()
		{
			if (m_class == PropClass::Matrix)
			{
				return (math::Matrix*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: matrix");
				return nullptr;
			}
		}

		Texture* MaterialProperty::GetTexture()
		{
			if (m_class == PropClass::Texture)
			{
				return (Texture*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: texture");
				return nullptr;
			}
		}

		Texture * MaterialProperty::GetSampler()
		{
			if (m_class == PropClass::TextureSampler)
			{
				return (Texture*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: texture sampler");
				return nullptr;
			}
		}

		ID3D11ShaderResourceView * MaterialProperty::GetResource()
		{
			if (m_class == PropClass::Resource)
			{
				return (ID3D11ShaderResourceView*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: resource");
				return nullptr;
			}
		}

		ID3D11Buffer * MaterialProperty::GetBuffer()
		{
			if (m_class == PropClass::Buffer)
			{
				return (ID3D11Buffer*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: buffer");
				return nullptr;
			}
		}

		ID3D11UnorderedAccessView * MaterialProperty::GetUAV()
		{
			if (m_class == PropClass::UAV)
			{
				return (ID3D11UnorderedAccessView*)m_value;
			}
			else
			{
				LOG("Material property: " << GetName() << " is not of type: Unordered access view");
				return nullptr;
			}
		}

	}
}

