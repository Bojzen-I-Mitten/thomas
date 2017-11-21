#include "Shader.h"
#include <AtlBase.h>
#include <atlconv.h>
#include <d3dcompiler.h>
#include "Shader.h"
namespace thomas
{
	namespace graphics
	{
		std::vector<Shader*> Shader::s_loadedShaders;

		Shader::Shader(std::string name, ID3DX11Effect* effect)
		{
			m_name = name;
			m_effect = effect;
			SetupReflection();
		}

		Shader::~Shader()
		{
			SAFE_RELEASE(m_effect);
			SAFE_RELEASE(m_inputLayout);
		}

		void Shader::SetupReflection()
		{
			D3DX11_EFFECT_DESC effectDesc;
			m_effect->GetDesc(&effectDesc);
			for (int i = 0; i < effectDesc.GlobalVariables; i++)
			{
				D3DX11_EFFECT_VARIABLE_DESC variableDesc;
				D3DX11_EFFECT_TYPE_DESC variableTypeDesc;
				m_effect->GetVariableByIndex(i)->GetType()->GetDesc(&variableTypeDesc);
				m_effect->GetVariableByIndex(i)->GetDesc(&variableDesc);

				ShaderVariable variable;
				variable.name = variableDesc.Name;
				variable.index = i;
				variable.desc = variableTypeDesc;

				m_variables.push_back(variable);

			}
			
			for (int i = 0; i < effectDesc.Techniques; i++)
			{
				D3DX11_TECHNIQUE_DESC techniqueDesc;
				ID3DX11EffectTechnique* tech = m_effect->GetTechniqueByIndex(i);
				tech->GetDesc(&techniqueDesc);
				for (int j = 0; j < techniqueDesc.Passes; j++)
				{
					D3DX11_PASS_SHADER_DESC vsPassDesc;
					D3DX11_EFFECT_SHADER_DESC vsDesc;
					ID3DX11EffectShaderVariable * vs;

					tech->GetPassByIndex(j)->GetVertexShaderDesc(&vsPassDesc);
					
					vs = vsPassDesc.pShaderVariable->AsShader();
					vs->GetShaderDesc(vsPassDesc.ShaderIndex, &vsDesc);

					std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

					for (int iInput = 0; iInput < vsDesc.NumInputSignatureEntries; ++iInput)
					{
						D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
						vs->GetInputSignatureElementDesc(vsPassDesc.ShaderIndex, iInput, &paramDesc);

						D3D11_INPUT_ELEMENT_DESC elementDesc;
						elementDesc.SemanticName = paramDesc.SemanticName;
						elementDesc.SemanticIndex = paramDesc.SemanticIndex;
						elementDesc.InputSlot = 0;
						elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
						elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
						elementDesc.InstanceDataStepRate = 0;

						// determine DXGI format
						elementDesc.Format = GetDXGIFormat(paramDesc.Mask, paramDesc.ComponentType);

						inputLayoutDesc.push_back(elementDesc);
		
					}
					HRESULT result = ThomasCore::GetDevice()->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), vsDesc.pBytecode, vsDesc.BytecodeLength, &m_inputLayout);

					if (result != S_OK)
					{
						LOG("Failed to create input layout for shader: " << m_name << " Error: " << result);
					}
				}
			}

		}

		DXGI_FORMAT Shader::GetDXGIFormat(BYTE mask, D3D_REGISTER_COMPONENT_TYPE componentType)
		{
			DXGI_FORMAT format;
			if (mask == 1)
			{
				if (componentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32_UINT;
				else if (componentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32_SINT;
				else if (componentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (mask <= 3)
			{
				if (componentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32_UINT;
				else if (componentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32_SINT;
				else if (componentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (mask <= 7)
			{
				if (componentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32B32_UINT;
				else if (componentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32B32_SINT;
				else if (componentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (mask <= 15)
			{
				if (componentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (componentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (componentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			else
			{
				format = DXGI_FORMAT_UNKNOWN;
				LOG("Unable to determine DXGI_FORMAT for shader: " << m_name);
			}
				

			return format;
		}

		bool Shader::Compile(std::string filePath, ID3DX11Effect** effect)
		{
			ID3DX11Effect* tempEffect;
			ID3DBlob* errorBlob;
			HRESULT result = D3DX11CompileEffectFromFile(
				CA2W(filePath.c_str()),
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				D3DCOMPILE_DEBUG,
				0,
				ThomasCore::GetDevice(),
				&tempEffect,
				&errorBlob);

			if (result != S_OK)
			{
				if (errorBlob)
				{
					if (errorBlob->GetBufferSize())
					{
						std::string error((char*)errorBlob->GetBufferPointer());
						LOG("Error compiling shader: " << filePath << ". errorBlob: " << error);
						errorBlob->Release();
					}
				}
				else
				{
					LOG("Error compiling shader: " << filePath << " error:" << result);
				}
				return false;
			}
			else if(errorBlob)
			{
				if (errorBlob->GetBufferSize())
				{
					//LOG("Shader Compiler : " << (char*)errorBlob->GetBufferPointer());
					errorBlob->Release();
				}
			}
			*effect = tempEffect;
			return true;
		}

		Shader * Shader::CreateShader(std::string name, std::string filePath)
		{
			ID3DX11Effect* effect = NULL;
			if (Compile(filePath, &effect))
			{
				Shader* shader = new Shader(name, effect);
				s_loadedShaders.push_back(shader);
				return shader;
			}
			return nullptr;
		}
		void Shader::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type)
		{
			ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(type);
		}
		void Shader::BindVertexBuffer(ID3D11Buffer * vertexBuffer, UINT stride, UINT offset)
		{
			ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		}
		void Shader::BindIndexBuffer(ID3D11Buffer * indexBuffer)
		{
			ThomasCore::GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		}
		void Shader::Bind()
		{
			ThomasCore::GetDeviceContext()->IASetInputLayout(m_inputLayout);
			//Currently only one pass. And one technique
			//TODO: More passes and techuniques
			m_effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, ThomasCore::GetDeviceContext());
		}
		std::string Shader::GetName()
		{
			return m_name;
		}
		void Shader::DestroyAllShaders()
		{
			for (int i = 0; i < s_loadedShaders.size(); i++)
			{
				delete s_loadedShaders[i];
			}
			s_loadedShaders.clear();
		}
		ID3DX11Effect * Shader::GetEffect()
		{
			return m_effect;
		}
		std::vector<Shader::ShaderVariable>* Shader::GetVariables()
		{
			return &m_variables;
		}
		int Shader::PropertyToID(const std::string & name)
		{
			for (int i = 0; i < m_variables.size(); i++)
			{
				if (m_variables[i].name == name)
					return i;
			}
			LOG("Could not find property: " << name << " for shader: " << m_name);
			return -1;
		}
	}
}