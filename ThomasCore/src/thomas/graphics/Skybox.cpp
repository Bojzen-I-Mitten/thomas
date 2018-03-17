#include "Skybox.h"
#include "Texture.h"
#include "LightManager.h"
#include "../utils/d3d.h"
#include "../ThomasCore.h"
#include "Material.h"
namespace thomas
{
	namespace graphics
	{

		Skybox::Skybox(std::string path, Material* material, int slot)
		{
			SetupBuffers();
			LoadCubeMap(path, slot);
			CreateRasterizer();
			CreateDepthStencilState();
			m_data.material = material;
		}


		Skybox::~Skybox()
		{
			SAFE_RELEASE(m_data.vertexBuffer);
			SAFE_RELEASE(m_data.indexBuffer);
			SAFE_RELEASE(m_data.constantBuffer);
			SAFE_RELEASE(m_data.rasterizerState);
			SAFE_RELEASE(m_data.depthStencilState);
		}

		bool Skybox::Bind(math::Matrix viewMatrix, math::Matrix mvpMatrix)
		{
			//Needs rework
			ThomasCore::GetDeviceContext()->RSGetState(&m_rasterizerP);
			ThomasCore::GetDeviceContext()->OMGetDepthStencilState(&m_depthstencilP, &m_depthRefP);

			m_data.material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			

			m_data.material->GetShader()->BindVertexBuffer(m_data.vertexBuffer, sizeof(math::Vector3), 0);
			m_data.material->GetShader()->BindIndexBuffer(m_data.indexBuffer);
			m_mvpStruct.mvpMatrix = mvpMatrix.Transpose();
			m_mvpStruct.viewMatrix = viewMatrix;
			viewMatrix.Invert().Decompose(math::Vector3(), math::Quaternion(),m_mvpStruct.camPosition);
			utils::D3d::FillDynamicBufferStruct(m_data.constantBuffer, m_mvpStruct);
			ThomasCore::GetDeviceContext()->RSSetState(m_data.rasterizerState);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(m_data.depthStencilState, 1);

			return false;
		}

		bool Skybox::BindCubemap()
		{
			
			return true;
		}

		bool Skybox::Unbind()
		{
			//needs rework

			return false;
		}

		void Skybox::Draw()
		{
			thomas::ThomasCore::GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);
		}

		void Skybox::SetupBuffers()
		{
			m_data.vertexBuffer = utils::D3d::CreateBufferFromVector(vertices, D3D11_BIND_VERTEX_BUFFER);

			if (m_data.vertexBuffer == nullptr)
				LOG("ERROR::INITIALIZING::VERTEX::BUFFER");


			m_data.indexBuffer = utils::D3d::CreateBufferFromVector(indices, D3D11_BIND_INDEX_BUFFER);

			if (m_data.indexBuffer == nullptr)
				LOG("ERROR::INITIALIZING::INDEX::BUFFER");

			m_data.constantBuffer = utils::D3d::CreateDynamicBufferFromStruct(m_mvpStruct, D3D11_BIND_CONSTANT_BUFFER);
		}

		void Skybox::CreateRasterizer()
		{
			m_data.rasterizerState = utils::D3d::CreateRasterizer(D3D11_FILL_SOLID, D3D11_CULL_NONE);
		}

		void Skybox::CreateDepthStencilState()
		{
			m_data.depthStencilState = utils::D3d::CreateDepthStencilState(D3D11_COMPARISON_LESS_EQUAL, true);
		}

		void Skybox::LoadCubeMap(std::string path, int slot)
		{
			m_data.texture.push_back(Texture::CreateTexture(Texture::SamplerState::WRAP, slot, Texture::TextureType::CUBEMAP, path));
		}
		void Skybox::BindSkyboxTexture()
		{
		
			return;
		}
		void Skybox::SetLerp(math::Vector3 lerp)
		{
			m_mvpStruct.lerp = lerp;
		}
	}
}