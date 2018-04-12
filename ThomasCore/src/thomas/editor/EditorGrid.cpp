#include "EditorGrid.h"
#include "../utils/d3d.h"
#include "../resource/Material.h"
#include "../utils/Buffers.h"
#include "../resource/Shader.h"
#include "../object/component/Camera.h"
#include "../graphics/Renderer.h"
namespace thomas
{
	namespace editor
	{
		void EditorGrid::AddLine(math::Vector3 from, math::Vector3 to, math::Vector4 color, float viewDistance)
		{


			m_lines.positions.push_back(math::Vector4(from.x, from.y, from.z, viewDistance));
			m_lines.colors.push_back(color);
	
			m_lines.positions.push_back(math::Vector4(to.x, to.y, to.z, viewDistance));
			m_lines.colors.push_back(color);

		}

		EditorGrid::EditorGrid(int gridSize, float cellSize, int internalGridSize)
		{
			m_gridSize = gridSize;
			m_cellSize = cellSize;
			m_internalGridSize = internalGridSize;

			m_lines.positions.reserve(gridSize*gridSize * 2 * 3);
			m_lines.colors.reserve(gridSize*gridSize * 2 * 3);

			for (int x = -m_gridSize / 2; x <= m_gridSize / 2; x++)
			{
				for (int z = -m_gridSize / 2; z <= m_gridSize / 2; z++)
				{
					math::Vector3 from(x, 0.0f, z);
					math::Vector3 to(x, 0.0f, z + 1);
					math::Vector3 to2(x + 1, 0.0f, z);
					if (z != m_gridSize)
						AddLine(from, to, math::Vector4(0.40625f, 0.40625f, 0.40625f, 0.8f));
					if (x != m_gridSize)
						AddLine(from, to2, math::Vector4(0.40625f, 0.40625f, 0.40625f, 0.8f));

					for (int xInternal = 0; xInternal < m_internalGridSize; xInternal++)
					{
						for (int zInternal = 0; zInternal < m_internalGridSize; zInternal++)
						{
							
							float internalCellSize = m_cellSize / m_internalGridSize;
							math::Vector3 internalFrom(xInternal, 0.0f, zInternal);
							internalFrom *= internalCellSize;
							internalFrom += from;
							math::Vector3 internalTo(xInternal, 0.0f, zInternal + 1);
							internalTo *= internalCellSize;
							internalTo += from;
							math::Vector3 internalTo2(xInternal + 1, 0.0f, zInternal);
							internalTo2 *= internalCellSize;
							internalTo2 += from;
							if (xInternal != 0)
								AddLine(internalFrom, internalTo, math::Vector4(0.3046875f, 0.3046875f, 0.3046875f, 0.2f), 0.3f);
							if(zInternal != 0)
								AddLine(internalFrom, internalTo2, math::Vector4(0.3046875f, 0.3046875f, 0.3046875f, 0.2f), 0.3f);
						}
					}
				}
			}

			resource::Shader* shader = resource::Shader::CreateShader("../Data/FXIncludes/EditorGridShader.fx");
			if (shader)
			{
				m_material = new resource::Material(shader);
				m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				m_mesh = new graphics::Mesh(m_lines, {}, "grid");
			}
				
			
		}

		void EditorGrid::Draw(object::component::Camera* camera)
		{
			if (m_material)
			{
				math::Vector3 cameraPos = camera->GetPosition();
				int scale = (int)log10(((abs(cameraPos.y/2)+1) / m_cellSize)*m_cellSize);
				scale = pow(10.0f,scale);
				math::Matrix worldMatrix = math::Matrix::CreateScale((scale)*m_cellSize) * math::Matrix::CreateTranslation(
					(int)(cameraPos.x / scale)*scale,
					0.0f,
					(int)(cameraPos.z / scale)*scale
				);
				scale *= m_cellSize;
				math::Vector4 cameraScaleMatrix(cameraPos.x, cameraPos.y, cameraPos.z, 0);
				m_material->SetVector("cameraPos", math::Vector4(cameraScaleMatrix));
				m_material->SetInt("gridScale", scale);

				graphics::Renderer::SubmitCommand(graphics::RenderCommand(worldMatrix, m_mesh, m_material, camera));
			}
			
		}

		EditorGrid::~EditorGrid()
		{
			for (auto buffer : m_vertexBuffers)
				delete buffer;
		}

	}
}
