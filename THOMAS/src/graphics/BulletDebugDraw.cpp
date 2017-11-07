#include "BulletDebugDraw.h"
#include "../ThomasCore.h"
#include "Shader.h"
#include "../utils/d3d.h"
#include "../object/component/Camera.h"
#include "DirectXTK/CommonStates.h"


namespace thomas
{
	namespace graphics
	{

		BulletDebugDraw::BulletDebugDraw()
		{

			//Create VS shader

			ID3D10Blob* blob = Shader::Compile("../res/thomasShaders/lineShader.hlsl", "vs_4_0", "VSMain");
			if (blob)
				ThomasCore::GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_vertexShader);

				

			//Create input layout
			std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
			layoutDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
	
			ThomasCore::GetDevice()->CreateInputLayout(&layoutDesc.front(), layoutDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayout);

			//Create PS shader
			SAFE_RELEASE(blob);
			blob = Shader::Compile("../res/thomasShaders/lineShader.hlsl", "ps_4_0", "PSMain");
			if (blob)
				ThomasCore::GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_pixelShader);

			SAFE_RELEASE(blob);
			LineVertex a;
			a.pos = math::Vector3(1, 0, 0);
			m_lines.push_back(a); //Line start
			m_lines.push_back(a); //Line end

			m_lineBuffer = utils::D3d::CreateDynamicBufferFromVector(m_lines, D3D11_BIND_VERTEX_BUFFER);
			m_cameraBuffer = utils::D3d::CreateDynamicBufferFromStruct(m_cameraData, D3D11_BIND_CONSTANT_BUFFER);

		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColor, const btVector3 & toColor)
		{

			DirectX::CommonStates states(ThomasCore::GetDevice());
			ThomasCore::GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
			ThomasCore::GetDeviceContext()->RSSetState(states.CullNone());
			
			ThomasCore::GetDeviceContext()->IASetInputLayout(m_inputLayout);

			ThomasCore::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

			ThomasCore::GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
			ThomasCore::GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);


			m_lines[0].pos = math::Vector3(from.x(), from.y(), from.z());
			m_lines[0].color = math::Vector3(fromColor.x(), fromColor.y(), fromColor.z());

			m_lines[1].pos = math::Vector3(to.x(), to.y(), to.z());
			m_lines[1].color = math::Vector3(toColor.x(), toColor.y(), toColor.z());

			UINT stride = sizeof(LineVertex);
			utils::D3d::FillDynamicBufferVector(m_lineBuffer, m_lines);
			
			UINT offset = 0;
			ThomasCore::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_cameraBuffer);
			ThomasCore::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_lineBuffer, &stride, &offset);

			ThomasCore::GetDeviceContext()->Draw(2, 0);

			ThomasCore::GetDeviceContext()->VSSetShader(NULL, NULL, 0);
			ThomasCore::GetDeviceContext()->PSSetShader(NULL, NULL, 0);

		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
		{
			drawLine(from, to, color, color);
		}

		void BulletDebugDraw::drawSphere(const btVector3 & p, btScalar radius, const btVector3 & color)
		{
		}

		void BulletDebugDraw::drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha)
		{
		}

		void BulletDebugDraw::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
		{
		}

		void BulletDebugDraw::reportErrorWarning(const char * warningString)
		{
		}

		void BulletDebugDraw::draw3dText(const btVector3 & location, const char * textString)
		{
		}

		void BulletDebugDraw::Update(object::component::Camera * camera)
		{
			m_cameraData.viewProjection = camera->GetViewProjMatrix().Transpose();
			utils::D3d::FillDynamicBufferStruct(m_cameraBuffer, m_cameraData);
			
		}


		void BulletDebugDraw::setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}

	}
}
