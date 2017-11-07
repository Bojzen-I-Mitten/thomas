#include "GeometryDraw.h"
#include "../object/component/Camera.h"
#include "../utils/d3d.h"
#include "../graphics/Shader.h"
#include "../ThomasCore.h"
#include <DirectXTK\CommonStates.h>
namespace thomas
{
	namespace graphics
	{
		std::vector<GeometryDraw*> GeometryDraw::s_geometry;
		GeometryDraw::GeometryDraw(math::Matrix worldMatrix)
		{
			m_inputLayout = nullptr;
			m_vertexShader = nullptr;
			m_geometryShader = nullptr;
			m_pixelShader = nullptr;
			VertexData a;
			a.position = math::Vector3(1, 0, 0);
			a.color = math::Vector3(1, 1, 1);
			for(int i = 0; i < 100; ++i)
				m_data.push_back(a);
			m_vertexBuffer = utils::D3d::CreateDynamicBufferFromVector(m_data, D3D11_BIND_VERTEX_BUFFER);
			m_constantBuffer = utils::D3d::CreateDynamicBufferFromStruct(m_cbData, D3D11_BIND_CONSTANT_BUFFER);
			m_cbData.worldMatrix = worldMatrix;
			s_geometry.push_back(this);
		}
		void GeometryDraw::SetShaders(std::string path, std::string shaderModel, std::string VSEntryPoint, std::string GSEntryPoint, std::string PSEntryPoint)
		{
			HRESULT hr;
			//Create VS
			//ID3D10Blob* blob = Shader::Compile(path, "vs" + shaderModel, VSEntryPoint);
			ID3D10Blob* blob = Shader::Compile("../res/thomasShaders/lineShader.hlsl", "vs_4_0", "VSMain");
			if (blob)
				hr = ThomasCore::GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_vertexShader);

			//Create input layout
			std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
			layoutDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			hr = ThomasCore::GetDevice()->CreateInputLayout(&layoutDesc.front(), layoutDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayout);

			//If GSEntryPoint Create GS
			if (GSEntryPoint != "")
			{
				SAFE_RELEASE(blob);
				blob = Shader::Compile(path, "gs" + shaderModel, GSEntryPoint);
				if (blob)
					ThomasCore::GetDevice()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_geometryShader);
			}

			//Create PS
			SAFE_RELEASE(blob);
			blob = Shader::Compile("../res/thomasShaders/lineShader.hlsl", "ps_4_0", "PSMain");
			if (blob)
				ThomasCore::GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_pixelShader);
			SAFE_RELEASE(blob);
		}
		void GeometryDraw::DrawLine(math::Vector3 from, math::Vector3 to, math::Vector3 fromColor, math::Vector3 toColor)
		{
			VertexData temp;
			temp.position = math::Vector3(from.x, from.y, from.z);
			temp.color = math::Vector3(fromColor.x, fromColor.y, fromColor.z);
			m_data.push_back(temp);
			temp.position = math::Vector3(to.x, to.y, to.z);
			temp.color = math::Vector3(toColor.x, toColor.y, toColor.z);
			m_data.push_back(temp);

		}
		void GeometryDraw::Draw()
		{
			for (GeometryDraw* geometry : s_geometry)
			{
				DirectX::CommonStates states(ThomasCore::GetDevice());
				/*ThomasCore::GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
				ThomasCore::GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
				ThomasCore::GetDeviceContext()->RSSetState(states.CullNone());*/

				ThomasCore::GetDeviceContext()->IASetInputLayout(geometry->m_inputLayout);
				ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				ThomasCore::GetDeviceContext()->VSSetShader(geometry->m_vertexShader, NULL, 0);
				ThomasCore::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
				if (geometry->m_geometryShader)
					ThomasCore::GetDeviceContext()->GSSetShader(geometry->m_geometryShader, NULL, 0);
				ThomasCore::GetDeviceContext()->PSSetShader(geometry->m_pixelShader, NULL, 0);
				UINT stride = sizeof(VertexData);
				if (geometry->m_data.size())
				{
					utils::D3d::FillDynamicBufferVector(geometry->m_vertexBuffer, geometry->m_data);

					UINT offset = 0;
					ThomasCore::GetDeviceContext()->VSSetConstantBuffers(0, 1, &geometry->m_constantBuffer);
					ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, 1, &geometry->m_vertexBuffer, &stride, &offset);

					ThomasCore::GetDeviceContext()->Draw(geometry->m_data.size(), 0); 
					geometry->m_data.clear();
				}

				ThomasCore::GetDeviceContext()->VSSetShader(NULL, NULL, 0);
				if (geometry->m_geometryShader)
					ThomasCore::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
				ThomasCore::GetDeviceContext()->PSSetShader(NULL, NULL, 0);
				ThomasCore::GetDeviceContext()->IASetInputLayout(NULL);
				ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//is this standard?
			}
		}
		void GeometryDraw::Update(object::component::Camera* camera)
		{
			m_cbData.viewProjectionMatrix = camera->GetViewProjMatrix().Transpose();
			utils::D3d::FillDynamicBufferStruct(m_constantBuffer, m_cbData);
		}
		void GeometryDraw::Destroy()
		{
			for (int i = 0; i < s_geometry.size(); ++i)
			{
				SAFE_RELEASE(s_geometry[i]->m_constantBuffer);
				SAFE_RELEASE(s_geometry[i]->m_geometryShader);
				SAFE_RELEASE(s_geometry[i]->m_inputLayout);
				SAFE_RELEASE(s_geometry[i]->m_pixelShader);
				SAFE_RELEASE(s_geometry[i]->m_vertexBuffer);
				SAFE_RELEASE(s_geometry[i]->m_vertexShader);
				delete s_geometry[i];	
			}
			s_geometry.clear();
		}
	}
}
