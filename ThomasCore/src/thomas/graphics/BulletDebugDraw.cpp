#include "BulletDebugDraw.h"
#include "../ThomasCore.h"
#include "../resource/Material.h"
#include "../utils/d3d.h"
#include "../object/component/Camera.h"
#include "DirectXTK/CommonStates.h"
#include "../resource/Shader.h"
#include "../editor/gizmos/Gizmos.h"
namespace thomas
{
	namespace graphics
	{

		BulletDebugDraw::BulletDebugDraw()
		{
			m_initialized = false;
			//Create VS shader
			resource::Shader* shader = resource::Shader::CreateShader("../Data/FXIncludes/lineShader.fx");
			if (shader != nullptr)
			{
				m_material = new resource::Material(shader);

				m_vertexBufferPos = new utils::buffers::VertexBuffer(nullptr, sizeof(math::Vector3), 1000, DYNAMIC_BUFFER);
				m_vertexBufferColor = new utils::buffers::VertexBuffer(nullptr, sizeof(math::Vector3), 1000, DYNAMIC_BUFFER);

				m_initialized = true;
			}
			

		}

		BulletDebugDraw::~BulletDebugDraw()
		{
			delete m_vertexBufferColor;
			delete m_vertexBufferPos;
		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColor, const btVector3 & toColor)
		{

			m_lines.positions.push_back(math::Vector3(from.x(), from.y(), from.z()));
			m_lines.colors.push_back(math::Vector3(fromColor.x(), fromColor.y(), fromColor.z()));

			m_lines.positions.push_back(math::Vector3(to.x(), to.y(), to.z()));
			m_lines.colors.push_back(math::Vector3(toColor.x(), toColor.y(), toColor.z()));
			

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

		void BulletDebugDraw::drawLineFinal()
		{
			if (m_lines.positions.empty())
				return;
			DirectX::CommonStates states(ThomasCore::GetDevice());
			ThomasCore::GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
			ThomasCore::GetDeviceContext()->RSSetState(states.CullNone());


			editor::Gizmos::SetMatrix(math::Matrix::Identity);
			editor::Gizmos::DrawLines(m_lines.positions);
			m_lines.positions.clear();
			m_lines.colors.clear();
		/*	m_vertexBufferPos->SetData(m_lines.positions);
			m_vertexBufferColor->SetData(m_lines.colors);

			m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			
			m_material->GetShader()->BindVertexBuffers({m_vertexBufferPos, m_vertexBufferColor});
						
			m_material->SetMatrix("viewProjection", m_viewProjection);

			m_material->Bind();
			m_material->Draw(m_lines.positions.size(), 0);
			m_lines.positions.clear();
			m_lines.colors.clear();*/
		}

	}
}
