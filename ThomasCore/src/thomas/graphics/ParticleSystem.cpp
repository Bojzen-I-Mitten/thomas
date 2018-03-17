#include "ParticleSystem.h"
#include "../object/component/ParticleEmitterComponent.h"
#include "../object/component/Transform.h"
#include "../object/component/Camera.h"
#include "../object/GameObject.h"
#include "ComputeShader.h"
#include "Renderer.h"
#include "../utils/d3d.h"
#include "../ThomasCore.h"
#include "../graphics/Texture.h"
#include "../ThomasTime.h"
#include "Mesh.h"

namespace thomas
{
	namespace graphics
	{
		ParticleSystem::CameraBufferStruct ParticleSystem::s_cameraBufferStruct;
		math::Matrix ParticleSystem::s_viewProjMatrix;
		ID3D11Buffer* ParticleSystem::s_cameraBuffer;

		ComputeShader* ParticleSystem::s_updateParticlesCS;
		ComputeShader* ParticleSystem::s_emitParticlesCS;

		ID3D11UnorderedAccessView* ParticleSystem::s_activeParticleUAV; //ping
		ID3D11ShaderResourceView* ParticleSystem::s_activeParticleSRV; //pong

		Mesh* ParticleSystem::s_emptyMesh;

		ParticleSystem::BlendStates ParticleSystem::s_blendStates;
		ID3D11DepthStencilState* ParticleSystem::s_depthStencilState;

		unsigned int ParticleSystem::s_maxNumberOfBillboardsSupported;


		ParticleSystem::ParticleSystem()
		{


		}

		ParticleSystem::~ParticleSystem()
		{
			s_depthStencilState->Release();
		}

		void ParticleSystem::Init()
		{
			s_maxNumberOfBillboardsSupported = 1000000;
			s_cameraBuffer = nullptr;
			
			s_emitParticlesCS = new ComputeShader(Shader::CreateShader("EmitParticlesCS", "../Data/oldShaders/emitParticlesCS.hlsl"));
			s_updateParticlesCS = new ComputeShader(Shader::CreateShader("UpdateParticlesCS", "../Data/oldShaders/updateParticlesCS.hlsl"));


			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(blendDesc));

			D3D11_RENDER_TARGET_BLEND_DESC blendState;
			ZeroMemory(&blendState, sizeof(blendState));

			//Additive

			blendState.BlendEnable = true;
			blendState.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendState.DestBlend = D3D11_BLEND_ONE;
			blendState.BlendOp = D3D11_BLEND_OP_ADD;
			blendState.SrcBlendAlpha = D3D11_BLEND_ONE;
			blendState.DestBlendAlpha = D3D11_BLEND_ZERO;
			blendState.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendState.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0] = blendState;

			HRESULT hr = ThomasCore::GetDevice()->CreateBlendState(&blendDesc, &s_blendStates.additive);

			//alpha blend
			blendState.BlendEnable = true;
			blendState.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendState.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendState.BlendOp = D3D11_BLEND_OP_ADD;
			blendState.SrcBlendAlpha = D3D11_BLEND_ONE;
			blendState.DestBlendAlpha = D3D11_BLEND_ZERO;
			blendState.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendState.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0] = blendState;

			hr = ThomasCore::GetDevice()->CreateBlendState(&blendDesc, &s_blendStates.alphaBlend);


			D3D11_DEPTH_STENCIL_DESC mirrorDesc;
			mirrorDesc.DepthEnable = true;
			mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
			mirrorDesc.StencilEnable = false;
			mirrorDesc.StencilReadMask = 0xff;
			mirrorDesc.StencilWriteMask = 0xff;
			mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				
			ThomasCore::GetDevice()->CreateDepthStencilState(&mirrorDesc, &s_depthStencilState);


			s_cameraBuffer = thomas::utils::D3d::CreateDynamicBufferFromStruct(s_cameraBufferStruct, D3D11_BIND_CONSTANT_BUFFER);
			return;
		}

		void ParticleSystem::Destroy()
		{
			SAFE_RELEASE(s_blendStates.additive);
			SAFE_RELEASE(s_blendStates.alphaBlend);
			SAFE_RELEASE(s_cameraBuffer);
			SAFE_RELEASE(s_depthStencilState);
		}

		void ParticleSystem::UpdateCameraBuffers(object::component::Transform* trans, math::Matrix viewProjMatrix, bool paused)
		{
			s_cameraBufferStruct.right = trans->Right();
			s_cameraBufferStruct.up = trans->Up();
			if (paused)
			{
				s_cameraBufferStruct.deltaTime = 0;
			}
			else
			{
				s_cameraBufferStruct.deltaTime = ThomasTime::GetDeltaTime();
			}
			
			s_viewProjMatrix = viewProjMatrix;

			thomas::utils::D3d::FillDynamicBufferStruct(s_cameraBuffer, s_cameraBufferStruct);

		}

		void ParticleSystem::SwapUAVsandSRVs(object::component::ParticleEmitterComponent * emitter)
		{
			object::component::ParticleEmitterComponent::D3DData* emitterD3D = emitter->GetD3DData();
			if (emitterD3D->swapUAVandSRV)
			{
				emitterD3D->swapUAVandSRV = false;
			}
			else
			{

				emitterD3D->swapUAVandSRV = true;
			}


			if (emitterD3D->swapUAVandSRV)
			{
				s_activeParticleUAV = emitterD3D->particleUAV1;
				s_activeParticleSRV = emitterD3D->particleSRV2;
			}
			else
			{
				s_activeParticleUAV = emitterD3D->particleUAV2;
				s_activeParticleSRV = emitterD3D->particleSRV1;
			}

		}

		void ParticleSystem::SpawnParticles(object::component::ParticleEmitterComponent * emitter, int amountOfParticles)
		{
			object::component::ParticleEmitterComponent::D3DData* emitterD3D = emitter->GetD3DData();
			
			s_emitParticlesCS->SetBuffer("InitBuffer", *emitterD3D->particleBuffer);
			s_emitParticlesCS->SetUAV("particlesWrite", *emitterD3D->particleUAV2);
			s_emitParticlesCS->SetUAV("particlesWrite2", *emitterD3D->particleUAV1);

			s_emitParticlesCS->Dispatch(amountOfParticles, 1, 1);

		}

		void ParticleSystem::UpdateParticles(object::component::ParticleEmitterComponent * emitter)
		{

			SwapUAVsandSRVs(emitter);

			//bind CS
			s_updateParticlesCS->SetUAV("particlesWrite", *s_activeParticleUAV);
			s_updateParticlesCS->SetUAV("billboards", *emitter->GetD3DData()->billboardsUAV);
			s_updateParticlesCS->SetResource("particlesRead", *s_activeParticleSRV);
			s_updateParticlesCS->SetBuffer("cameraBuffer", *s_cameraBuffer);

			s_updateParticlesCS->Dispatch(emitter->GetSpawnedParticleCount() / 256 + 1, 1, 1);

		}

		void ParticleSystem::DrawParticles(object::component::Camera * camera, object::component::ParticleEmitterComponent* emitter)
		{
			UpdateCameraBuffers(camera->m_gameObject->m_transform, camera->GetViewProjMatrix().Transpose(), emitter->IsPaused());
			UpdateParticles(emitter);

			FLOAT blendfactor[4] = { 0, 0, 0, 0 };
			
			switch (emitter->GetBlendState())
			{
			case object::component::ParticleEmitterComponent::BlendStates::ADDITIVE:
				ThomasCore::GetDeviceContext()->OMSetBlendState(s_blendStates.additive, blendfactor, 0xffffffff);
				break;
			case object::component::ParticleEmitterComponent::BlendStates::ALPHA_BLEND:
				ThomasCore::GetDeviceContext()->OMSetBlendState(s_blendStates.alphaBlend, blendfactor, 0xffffffff);
				break;
			default:
				ThomasCore::GetDeviceContext()->OMSetBlendState(s_blendStates.additive, blendfactor, 0xffffffff);
				break;
			}

			//bind Emitter

			emitter->GetMaterial()->SetResource("particle", *emitter->GetD3DData()->billboardsSRV);
			emitter->GetMaterial()->SetMatrix("matrixBuffer", s_viewProjMatrix);
			emitter->GetMaterial()->m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			emitter->GetMaterial()->GetShader()->BindVertexBuffer(NULL, 0, 0);

			emitter->GetMaterial()->Draw(emitter->GetNrOfMaxParticles() * 6, 0);

			

			ThomasCore::GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);

		}

		ID3D11DepthStencilState * ParticleSystem::GetDepthStencilState()
		{
			return s_depthStencilState;
		}

		void ParticleSystem::CreateBillboardUAVandSRV(int maxAmountOfParticles, ID3D11Buffer*& buffer, ID3D11UnorderedAccessView*& uav, ID3D11ShaderResourceView*& srv)
		{
			UINT bytewidth = sizeof(BillboardStruct) * maxAmountOfParticles;

			UINT structurebytestride = sizeof(BillboardStruct);
			thomas::utils::D3d::CreateBufferAndUAV(NULL, bytewidth, structurebytestride, buffer, uav, srv);

		}



	}
}

