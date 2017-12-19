#include "GizmoRender.h"
#include "../../graphics/Material.h"
#include "../../graphics/Shader.h"
#include "../EditorCamera.h"
#include "../../utils/d3d.h"
#include "../../utils/AssimpLoader.h"
#include "../../graphics/Model.h"
#include "Gizmos.h"
namespace thomas
{
	namespace editor
	{
		graphics::Material* GizmoRender::s_material = nullptr;
		graphics::Model* GizmoRender::s_cone;
		void GizmoRender::Init()
		{
			s_cone = utils::AssimpLoader::LoadModel("primitiveCone", "../Data/cone.obj", "bl�");
			graphics::Shader* shader = graphics::Shader::CreateShader("editorGrid", "../Data/FXIncludes/EditorGizmoShader.fx");
			if (shader)
			{
				s_material = new graphics::Material(shader);

			}
		}

		void GizmoRender::DrawAxis(const math::Vector3 & origin, const math::Vector3 & axis , const math::Vector3& perp, const math::Color& color)
		{
			if (!s_material)
				return;
			//Draw line
			UINT stride = sizeof(LineVertex);
			UINT offset = 0;
			std::vector<LineVertex> lines;
			lines.push_back(LineVertex{ math::Vector3() }); //start
			lines.push_back(LineVertex{ math::Vector3::Up }); //axis out from start
			
			s_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			
			ID3D11Buffer* lineBuffer = utils::D3d::CreateDynamicBufferFromVector(lines, D3D11_BIND_VERTEX_BUFFER);

			float scale = math::Vector3::Distance(EditorCamera::GetEditorCamera()->m_transform->GetPosition(), origin)/5.0f;
				

			math::Matrix world = math::Matrix::CreateScale(scale) * math::Matrix::CreateWorld(origin, perp, axis);
			s_material->GetShader()->BindVertexBuffer(lineBuffer, stride, offset);
			s_material->SetMatrix("thomas_ObjectToWorld", world.Transpose());
			s_material->SetColor("color", color);
			s_material->Bind();
			s_material->Draw(lines.size(), 0);
			

			s_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			world = math::Matrix::CreateScale(scale * 0.1f) * math::Matrix::CreateWorld(origin + axis*scale, perp, axis);
			s_material->SetMatrix("thomas_ObjectToWorld", world.Transpose());
			
			s_material->Bind();
			s_material->Draw(s_cone->GetMeshes()[0]);
			SAFE_RELEASE(lineBuffer);

			Gizmos::SetColor(color);
			Gizmos::SetMatrix(math::Matrix::Identity);
		/*	math::Vector3 extends = (axis + math::Vector3(0.1,0.1,0.1))*scale / 2.0f;
			math::Vector3 center = origin + axis*scale*0.6f;
			
			math::BoundingOrientedBox box(center, extends,math::Quaternion::Identity);*/
			//Gizmos::DrawBoundingOrientedBox(box);
			math::BoundingSphere AxisBounds(1.1f*scale*axis + origin, scale*0.2f);
			Gizmos::DrawBoundingSphere(AxisBounds);
		

			/*for (int i = 0; i <= 30; i++)
			{
				tvector3 pt;
				pt = vtx * cos(((2 * ZPI) / 30.0f)*i)*fct;
				pt += vty * sin(((2 * ZPI) / 30.0f)*i)*fct;
				pt += axis*fct2;
				pt += orig;
				glVertex3fv(&pt.x);
				pt = vtx * cos(((2 * ZPI) / 30.0f)*(i + 1))*fct;
				pt += vty * sin(((2 * ZPI) / 30.0f)*(i + 1))*fct;
				pt += axis*fct2;
				pt += orig;
				glVertex3fv(&pt.x);
				glVertex3f(orig.x + axis.x, orig.y + axis.y, orig.z + axis.z);

			}*/
		}

	}
}
