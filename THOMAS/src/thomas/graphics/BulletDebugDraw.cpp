#include "BulletDebugDraw.h"
#include "../ThomasCore.h"
#include "Material.h"
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
			Shader* shader = Shader::CreateShader("debugDraw", "../res/thomasShaders/lineShader.hlsl");
			if (shader != nullptr)
			{
				m_material = new Material(shader);
				

				LineVertex a;
				a.pos = math::Vector3(1, 0, 0);
				m_lines.push_back(a); //Line start
				m_lines.push_back(a); //Line end

				m_vertexBuffer = utils::D3d::CreateDynamicBufferFromVector(m_lines, D3D11_BIND_VERTEX_BUFFER);
			}
			

		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColor, const btVector3 & toColor)
		{

			DirectX::CommonStates states(ThomasCore::GetDevice());
			ThomasCore::GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
			ThomasCore::GetDeviceContext()->RSSetState(states.CullNone());
			
			m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

			m_lines[0].pos = math::Vector3(from.x(), from.y(), from.z());
			m_lines[0].color = math::Vector3(fromColor.x(), fromColor.y(), fromColor.z());

			m_lines[1].pos = math::Vector3(to.x(), to.y(), to.z());
			m_lines[1].color = math::Vector3(toColor.x(), toColor.y(), toColor.z());



			UINT stride = sizeof(LineVertex);
			UINT offset = 0;
			utils::D3d::FillDynamicBufferVector(m_vertexBuffer, m_lines);
			m_material->GetShader()->BindVertexBuffer(m_vertexBuffer, stride, offset);

			m_material->SetMatrix("viewProjection", m_viewProjection);
			
			m_material->Draw(2, 0);

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
			m_viewProjection = camera->GetViewProjMatrix().Transpose();
			
		}


		void BulletDebugDraw::setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}

	}
}
