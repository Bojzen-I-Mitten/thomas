#include "Shader.h"
#include <AtlBase.h>
#include <atlconv.h>
#include <d3dcompiler.h>
#include "Shader.h"
#include "ShaderProperty\shaderProperties.h"
#include <fstream>
namespace thomas
{
	namespace resource
	{
		std::vector<Shader*> Shader::s_loadedShaders;
		Shader* Shader::s_standardShader;
		Shader* Shader::s_failedShader;
		bool Shader::s_shouldRecompile = false;
		Shader::Shader(ID3DX11Effect* effect, std::string path) : Resource(path)
		{
			m_currentPass = nullptr;
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
					AddProperty(variable);
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
						std::vector<Semantics> inputSemantics;
						for (int iInput = 0; iInput < vsDesc.NumInputSignatureEntries; iInput++)
						{
							D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
							vs->GetInputSignatureElementDesc(vsPassDesc.ShaderIndex, iInput, &paramDesc);

							Semantics semantic = GetSemanticFromName(paramDesc.SemanticName);

							D3D11_INPUT_ELEMENT_DESC elementDesc;
							elementDesc.SemanticName = paramDesc.SemanticName;
							elementDesc.SemanticIndex = paramDesc.SemanticIndex;
							elementDesc.InputSlot = iInput;
							elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
							elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							elementDesc.InstanceDataStepRate = 0;

							// determine DXGI format
							elementDesc.Format = GetDXGIFormat(paramDesc.Mask, paramDesc.ComponentType);
							
							inputLayoutDesc.push_back(elementDesc);
							inputSemantics.push_back(semantic);
						}

						HRESULT result = ThomasCore::GetDevice()->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), vsDesc.pBytecode, vsDesc.BytecodeLength, &pass.inputLayout);
						pass.inputSemantics = inputSemantics;
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
					std::string error = (char*)errorBlob->GetBufferPointer();
					//LOG("Shader Compiler : " << error);
					errorBlob->Release();
				}
				
			}
			*effect = tempEffect;
			return true;
		}

		bool Shader::Init()
		{
			s_failedShader = CreateShader("../Data/FXIncludes/FailedShader.fx");
			s_standardShader = CreateShader("../Data/FXIncludes/StandardShader.fx");
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
			
			if (!Compile(path, &effect))
				Compile(s_failedShader->m_path, &effect);

			Shader* shader = new Shader(effect, path);
			s_loadedShaders.push_back(shader);
			if (shader->m_passes.empty())
			{
				LOG("shader: " << path << " contains no techniques or passes");
				
			}
			return shader;
		}
		void Shader::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY type)
		{
			ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(type);
		}
		void Shader::BindVertexBuffer(utils::buffers::VertexBuffer* buffer)
		{
			size_t stride = buffer->GetStride();
			ID3D11Buffer* buff = buffer->GetBuffer();
			size_t offset = 0;
			ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
		}

		void Shader::BindVertexBuffers(std::vector<utils::buffers::VertexBuffer*> buffers)
		{
			std::vector<ID3D11Buffer*> buffs;
			std::vector<size_t> strides;
			std::vector<size_t> offsets;

			for (utils::buffers::VertexBuffer* buffer : buffers)
			{
				buffs.push_back(buffer->GetBuffer());
				strides.push_back(buffer->GetStride());
				offsets.push_back(0);
			}

			ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, buffs.size(), buffs.data(), strides.data(), offsets.data());
		}


		void Shader::BindIndexBuffer(utils::buffers::IndexBuffer* indexBuffer)
		{
			ThomasCore::GetDeviceContext()->IASetIndexBuffer(indexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
		}
		void Shader::Bind()
		{
			for (auto prop : m_properties)
				prop.second->Apply(this);
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
			m_currentPass = &m_passes[passIndex];
		}
		Shader::ShaderPass & Shader::GetCurrentPass()
		{
			return *m_currentPass;
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
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyColor(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalFloat(const std::string & name, float value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyScalarFloat(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalInt(const std::string & name, int value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyScalarInt(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalMatrix(const std::string & name, math::Matrix value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyMatrix(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalTexture2D(const std::string & name, resource::Texture2D* value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyTexture2D(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalResource(const std::string & name, ID3D11ShaderResourceView * value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyShaderResource(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		void Shader::SetGlobalConstantBuffer(const std::string & name, ID3D11Buffer * value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyConstantBuffer(value));
					shader->m_properties[name]->SetName(name);
				}
			}
		}
		
		void Shader::SetGlobalVector(const std::string & name, math::Vector4 value)
		{
			for (auto shader : s_loadedShaders)
			{
				if (shader->HasProperty(name))
				{
					shader->m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(new shaderProperty::ShaderPropertyVector(value));
					shader->m_properties[name]->SetName(name);
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

		std::vector<std::string> Shader::GetMaterialProperties()
		{
			return m_materialProperties;
		}

		ID3DX11Effect * Shader::GetEffect()
		{
			return m_effect;
		}

		bool Shader::HasProperty(const std::string & name)
		{
			return m_properties.find(name) != m_properties.end();
		}
		std::shared_ptr<shaderProperty::ShaderProperty> Shader::GetProperty(const std::string & name)
		{
			for (auto& prop : m_properties)
			{
				if (prop.first == name)
					return prop.second;
			}
			return nullptr;
		}

		std::map<std::string, std::shared_ptr<shaderProperty::ShaderProperty>> Shader::GetProperties()
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
				m_materialProperties.clear();
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
		Shader::Semantics Shader::GetSemanticFromName(std::string semanticName)
		{
			if (semanticName.find("BINORMAL") != std::string::npos)
			{
				return Semantics::BINORMAL;
			}
			else if (semanticName.find("COLOR") != std::string::npos)
			{
				return Semantics::COLOR;
			}
			else if (semanticName.find("BLENDINDICES") != std::string::npos)
			{
				return Semantics::BLENDINDICES;
			}
			else if (semanticName.find("BLENDWEIGHT") != std::string::npos)
			{
				return Semantics::BLENDWEIGHT;
			}
			else if (semanticName.find("NORMAL") != std::string::npos)
			{
				return Semantics::NORMAL;
			}
			else if (semanticName.find("POSITION") != std::string::npos)
			{
				return Semantics::POSITION;
			}
			else if (semanticName.find("POSITIONT") != std::string::npos)
			{
				return Semantics::POSITIONT;
			}
			else if (semanticName.find("PSIZE") != std::string::npos)
			{
				return Semantics::PSIZE;
			}
			else if (semanticName.find("TANGENT") != std::string::npos)
			{
				return Semantics::TANGENT;
			}
			else if (semanticName.find("TEXCOORD") != std::string::npos)
			{
				return Semantics::TEXCOORD;
			}
			else if (semanticName.find("BITANGENT") != std::string::npos)
			{
				return Semantics::BITANGENT;
			}
			else
			{
				return Semantics::UNKNOWN;
			}
			
		}
		void Shader::AddProperty(ID3DX11EffectVariable * prop)
		{
			D3DX11_EFFECT_TYPE_DESC typeDesc;
			D3DX11_EFFECT_VARIABLE_DESC variableDesc;

			prop->GetType()->GetDesc(&typeDesc);
			prop->GetDesc(&variableDesc);
			ID3DX11EffectConstantBuffer* cBuffer = prop->GetParentConstantBuffer();
			
			bool isMaterialProperty = false;
			shaderProperty::ShaderProperty* newProperty = nullptr;
			std::string semantic;
			if (variableDesc.Semantic != NULL)
				semantic = variableDesc.Semantic;
			switch (typeDesc.Class)
			{
			case D3D_SVC_SCALAR:
			{
				switch (typeDesc.Type)
				{
				case D3D_SVT_BOOL:
					newProperty = shaderProperty::ShaderPropertyScalarBool::GetDefault();
					break;
				case D3D_SVT_INT:
				case D3D_SVT_UINT:
					newProperty = shaderProperty::ShaderPropertyScalarInt::GetDefault();
					break;
				case D3D_SVT_FLOAT:
					newProperty = shaderProperty::ShaderPropertyScalarFloat::GetDefault();
					break;
				default:
					break;
				}
				break;
			}
			case D3D_SVC_VECTOR:
				if(semantic == "COLOR")
					newProperty = shaderProperty::ShaderPropertyColor::GetDefault();
				else
					newProperty = shaderProperty::ShaderPropertyVector::GetDefault();
				break;
			case D3D_SVC_MATRIX_COLUMNS:
			case D3D_SVC_MATRIX_ROWS:
				newProperty = shaderProperty::ShaderPropertyMatrix::GetDefault();
				break;
			case D3D_SVC_OBJECT:
			{
				switch (typeDesc.Type)
				{
				case D3D_SVT_CBUFFER:
					newProperty = shaderProperty::ShaderPropertyConstantBuffer::GetDefault();
					break;
				//case D3D_SVT_TEXTURE:
				//case D3D_SVT_TEXTURE1D:
				//case D3D_SVT_RWTEXTURE1D:
				case D3D_SVT_TEXTURE2DMS:
				case D3D_SVT_RWTEXTURE2D:
				case D3D_SVT_TEXTURE2D:
				//case D3D_SVT_RWTEXTURE3D:
				//case D3D_SVT_TEXTURE3D:
				//case D3D_SVT_TEXTURECUBE:
					isMaterialProperty = true;
					newProperty = shaderProperty::ShaderPropertyTexture2D::GetDefault();
					break;
				case D3D_SVT_STRUCTURED_BUFFER:
				case D3D_SVT_RWSTRUCTURED_BUFFER:
				case D3D_SVT_APPEND_STRUCTURED_BUFFER:
				case D3D_SVT_CONSUME_STRUCTURED_BUFFER:
					newProperty = shaderProperty::ShaderPropertyShaderResource::GetDefault();
				}
				break;
			}
			default:
				break;
			}
			
			std::string name = variableDesc.Name;
			
			if (cBuffer->IsValid())
			{
				D3DX11_EFFECT_VARIABLE_DESC bufferDesc;
				cBuffer->GetDesc(&bufferDesc);
				std::string bufferName = bufferDesc.Name;
				if (bufferName == "MATERIAL_PROPERTIES")
					isMaterialProperty = true;
			}
			if (newProperty != nullptr)
			{
				newProperty->SetName(name);
				if(!HasProperty(name))
					m_properties[name] = std::shared_ptr<shaderProperty::ShaderProperty>(newProperty);
				if(isMaterialProperty)
					m_materialProperties.push_back(name);
			}
			
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