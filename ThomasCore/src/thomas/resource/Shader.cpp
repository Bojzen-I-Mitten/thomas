#include "Shader.h"
#include <AtlBase.h>
#include <atlconv.h>
#include <d3dcompiler.h>
#include "Shader.h"
#include "ShaderProperty.h"
#include <fstream>
namespace thomas
{
	namespace resource
	{
		std::vector<Shader*> Shader::s_loadedShaders;
		Shader* Shader::s_standardShader;
		bool Shader::s_shouldRecompile = false;
		Shader::Shader(ID3DX11Effect* effect, std::string path) : Resource(path)
		{
			m_effect = effect;
			SetupReflection();
		}

		Shader::~Shader()
		{
			Destroy();
		}

		void Shader::SetupReflection()
		{
			D3DX11_EFFECT_DESC effectDesc;
			m_effect->GetDesc(&effectDesc);

			if (effectDesc.Techniques == 0)
			{
				return;
			}

			for (int i = 0; i < effectDesc.GlobalVariables; i++)
			{
				ID3DX11EffectVariable* variable = m_effect->GetVariableByIndex(i);
				if (variable->IsValid())
				{
					if (m_properties.size() > i)
					{
						m_properties[i]->UpdateVariable(variable);
					}else
					{ 
						ShaderProperty* prop = new ShaderProperty(i, variable);
						m_properties.push_back(prop);
					}
					
				}
				

			}
			
			D3DX11_TECHNIQUE_DESC techniqueDesc;
			ID3DX11EffectTechnique* tech = m_effect->GetTechniqueByIndex(0);
			if (tech->IsValid())
			{
				tech->GetDesc(&techniqueDesc);
				for (int j = 0; j < techniqueDesc.Passes; j++)
				{
					ShaderPass pass;
					D3DX11_PASS_SHADER_DESC vsPassDesc;
					D3DX11_EFFECT_SHADER_DESC vsDesc;
					D3DX11_PASS_DESC passDesc;
					ID3DX11EffectShaderVariable * vs;

					tech->GetPassByIndex(j)->GetDesc(&passDesc);
					tech->GetPassByIndex(j)->GetVertexShaderDesc(&vsPassDesc);

					pass.name = passDesc.Name;
					pass.inputLayout = NULL;

					vs = vsPassDesc.pShaderVariable->AsShader();
					if (vs->IsValid())
					{
						vs->GetShaderDesc(vsPassDesc.ShaderIndex, &vsDesc);

						std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

						for (int iInput = 0; iInput < vsDesc.NumInputSignatureEntries; iInput++)
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

						HRESULT result = ThomasCore::GetDevice()->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), vsDesc.pBytecode, vsDesc.BytecodeLength, &pass.inputLayout);

						if (result != S_OK)
						{
							LOG("Failed to create input layout for shader: " << m_path << " Error: " << result);
						}
					}
					m_passes.push_back(pass);
					
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
				LOG("Unable to determine DXGI_FORMAT for shader: " << m_path);
			}
				

			return format;
		}

		void Shader::Destroy()
		{
			SAFE_RELEASE(m_effect);
			for (auto pass : m_passes)
				SAFE_RELEASE(pass.inputLayout);
			m_passes.clear();
			for (ShaderProperty* mProp : m_properties)
				delete mProp;
			m_properties.clear();
		}

		bool Shader::Compile(std::string filePath, ID3DX11Effect** effect)
		{
			size_t found = filePath.find_last_of("/\\");
			std::string dir = filePath.substr(0, found);
			
			Shader::ShaderInclude include(dir.c_str(), "..\\Data\\FxIncludes");

			ID3DX11Effect* tempEffect = nullptr;
			ID3DBlob* errorBlob = nullptr;
			HRESULT result = D3DX11CompileEffectFromFile(
				CA2W(filePath.c_str()),
				nullptr,
				&include,
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
					std::string error = HR_TO_STRING(result);
					LOG("Error compiling shader: " << filePath << " error: " << error);
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

		bool Shader::Init()
		{
			s_standardShader = CreateShader("../Data/FXIncludes/StandardShader.fx");
			if (!s_standardShader)
				return false;
			return true;
		}

		Shader * Shader::GetStandardShader()
		{
			return s_standardShader;
		}

		Shader * Shader::CreateShader(std::string path)
		{
			Shader* foundShader = FindByName(PathToName(path));
			if (foundShader)
				return foundShader;

			ID3DX11Effect* effect = NULL;
			if (Compile(path, &effect))
			{
				Shader* shader = new Shader(effect, path);
				if (!shader->m_passes.empty())
				{
					s_loadedShaders.push_back(shader);
					return shader;
				}
				else
				{
					SAFE_RELEASE(effect);
					return nullptr;
					LOG("Can't create shader: " << path << " because it contains no techniques or passes");
				}	
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
			for (auto prop : m_properties)
				prop->ApplyProperty(this);
		}
		std::vector<Shader::ShaderPass>* Shader::GetPasses()
		{
			return &m_passes;
		}
		
		void Shader::SetPass(int passIndex)
		{
			ThomasCore::GetDeviceContext()->IASetInputLayout(m_passes[passIndex].inputLayout);
			ID3DX11EffectPass* pass = m_effect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
			pass->Apply(0, ThomasCore::GetDeviceContext());
		}
		void Shader::DestroyAllShaders()
		{
			for (int i = 0; i < s_loadedShaders.size(); i++)
			{
				delete s_loadedShaders[i];
			}
			s_loadedShaders.clear();
		}

		void Shader::SetGlobalColor(const std::string & name, math::Color value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetVector(value.ToVector4());
				}
			}
		}
		void Shader::SetGlobalFloat(const std::string & name, float value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetFloat(value);
				}
			}
		}
		void Shader::SetGlobalInt(const std::string & name, int value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetInt(value);
				}
			}
		}
		void Shader::SetGlobalMatrix(const std::string & name, math::Matrix value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetMatrix(value);
				}
			}
		}
		void Shader::SetGlobalTexture(const std::string & name, graphics::Texture & value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetTexture(value);
				}
			}
		}
		void Shader::SetGlobalVector(const std::string & name, math::Vector4 value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->GetProperty(name)->SetVector(value);
				}
			}
		}

		Shader * Shader::FindByName(const std::string & name)
		{
			for (Shader* shader : s_loadedShaders)
			{
				if (shader->GetName() == name)
					return shader;
			}
			return nullptr;
		}

		Shader * Shader::FindByPath(const std::string& path)
		{
			for (Shader* shader : s_loadedShaders)
			{
				if (shader->m_path == path)
					return shader;
			}
			return nullptr;
		}

		ID3DX11Effect * Shader::GetEffect()
		{
			return m_effect;
		}

		bool Shader::HasProperty(const std::string & name)
		{
			for (ShaderProperty* prop : m_properties)
			{
				if (prop->GetName() == name)
					return true;
			}
			return false;
		}
		ShaderProperty * Shader::GetProperty(const std::string & name)
		{
			for (ShaderProperty* prop : m_properties)
			{
				if (prop->GetName() == name)
					return prop;
			}
			return nullptr;
		}

		std::vector<ShaderProperty*> Shader::GetProperties()
		{
			return m_properties;
		}

		void Shader::Update()
		{
			if (s_shouldRecompile)
			{
				LOG("Recompiling Shaders...");
				RecompileShaders();
				s_shouldRecompile = false;
			}
				
		}

		void Shader::Recompile()
		{
			ID3DX11Effect* tempEffect;
			
			if (Compile(m_path, &tempEffect))
			{
				SAFE_RELEASE(m_effect);
				for (auto pass : m_passes)
					SAFE_RELEASE(pass.inputLayout);

				m_passes.clear();
				m_effect = tempEffect;
				SetupReflection();
			}
			else
			{
				LOG("Could not recompile shader " << m_path);
			}

		}

		void Shader::RecompileShaders()
		{
			for (Shader* shader : s_loadedShaders)
			{
				shader->Recompile();
			}
		}
		void Shader::OnChanged()
		{
			Recompile();
		}
		void Shader::QueueRecompile()
		{
			s_shouldRecompile = true;
		}


		Shader::ShaderInclude::ShaderInclude(const char * shaderDir, const char * systemDir) : m_shaderDir(shaderDir), m_systemDir(systemDir)
		{
		}

		HRESULT Shader::ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes)
		{
			try
			{
				
				std::string finalPath;
				std::string dir;
				switch (IncludeType)
				{
				case D3D_INCLUDE_LOCAL: // #include "FILE"
					dir = m_shaderDir;
					break;
				case D3D_INCLUDE_SYSTEM: // #include <FILE>
					dir = m_systemDir;
					break;
				default:
					assert(0);
				}

				/*
				If pFileName is absolute: finalPath = pFileName.
				If pFileName is relative: finalPath = dir + "\\" + pFileName
				*/
				if (PathIsRelative(pFileName))
				{
					finalPath = dir + "\\" + pFileName;
				}
				else
				{
					finalPath = pFileName;
				}
				
				std::ifstream fileStream(finalPath, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
				if (fileStream.fail())
				{
					LOG("Failed to find shader: " << finalPath);
					return E_FAIL;
				}
				uint32_t fileSize = fileStream.tellg();

				if (fileSize)
				{
					char* buffer = new char[fileSize];
					fileStream.seekg(0, std::ifstream::beg);
					fileStream.read(buffer, fileSize);

					*ppData = buffer;
					*pBytes = fileSize;
				}
				else
				{
					*ppData = nullptr;
					*pBytes = 0;
				}
				return S_OK;
			}
			catch (std::exception e)
			{
				LOG("Failed to read shader include: " << pFileName << " error: " << e.what());
				return E_FAIL;
			}
		}
		HRESULT Shader::ShaderInclude::Close(LPCVOID pData)
		{
			char* bufffer = (char*)pData;
			delete[] bufffer;
			return S_OK;
		}
	}
}