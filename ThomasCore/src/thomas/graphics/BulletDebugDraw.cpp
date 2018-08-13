#include "BulletDebugDraw.h"
#include "DirectXTK\CommonStates.h"
#include "..\ThomasCore.h"
#include "..\resource\Material.h"
#include "..\utils\D3D.h"
#include "..\object\component\Camera.h"
#include "..\resource\Shader.h"

namespace thomas
{
	namespace graphics
	{
		BulletDebugDraw::BulletDebugDraw()
		{
			resource::Shader* shader = resource::Shader::CreateShader("../Data/FXIncludes/lineShader.fx"); // Hardcoded line shader for debug draw

			if (shader != nullptr)
			{
				m_material = std::make_unique<resource::Material>(shader);
				m_vertexBufferPos = std::make_unique<utils::buffers::VertexBuffer>(nullptr, sizeof(math::Vector3), 1000, DYNAMIC_BUFFER);
				m_vertexBufferColor = std::make_unique<utils::buffers::VertexBuffer>(nullptr, sizeof(math::Vector3), 1000, DYNAMIC_BUFFER);
			}
		}

		BulletDebugDraw::~BulletDebugDraw()
		{
			m_material.reset();
			m_vertexBufferPos.reset();
			m_vertexBufferColor.reset();
		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColor, const btVector3 & toColor)
		{
			m_linePositions.push_back(math::Vector3(from.x(), from.y(), from.z()));
			m_linePositions.push_back(math::Vector3(to.x(), to.y(), to.z()));
			m_lineColors.push_back(math::Vector3(fromColor.x(), fromColor.y(), fromColor.z()));
			m_lineColors.push_back(math::Vector3(toColor.x(), toColor.y(), toColor.z()));	
		}

		void BulletDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
		{
			drawLine(from, to, color, color);
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
			if (m_linePositions.empty())
				return;

			// Set necessary states
			DirectX::CommonStates states(ThomasCore::GetDevice());
			ThomasCore::GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
			ThomasCore::GetDeviceContext()->RSSetState(states.CullNone());

			// Set the data and send to GPU
			m_vertexBufferPos->SetData(m_linePositions);
			m_vertexBufferColor->SetData(m_lineColors);
			m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;		
			m_material->GetShader()->BindVertexBuffers({m_vertexBufferPos.get(), m_vertexBufferColor.get()});	
			m_material->SetMatrix("viewProjection", m_viewProjection);
			m_material->Bind();
			m_material->Draw(m_linePositions.size(), 0);

			// Clear the memory
			m_linePositions.clear();
			m_lineColors.clear();
		}
	}
}
