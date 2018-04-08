#include "EditorGrid.h"
#include "../utils/d3d.h"
#include "../graphics/Material.h"
#include "../resource/Shader.h"
namespace thomas
{
	namespace editor
	{
		void EditorGrid::AddLine(math::Vector3 from, math::Vector3 to, math::Vector4 color, float viewDistance)
		{
			LineVertex fromLine;
			LineVertex toLine;

	
			fromLine.pos = from;
			fromLine.color = color;
			fromLine.viewDistance = viewDistance;
			toLine.pos = to;
			toLine.color = color;
			toLine.viewDistance = viewDistance;
			m_lines.push_back(fromLine);
			m_lines.push_back(toLine);
		}

		EditorGrid::EditorGrid(int gridSize, float cellSize, int internalGridSize)
		{
			m_gridSize = gridSize;
			m_cellSize = cellSize;
			m_internalGridSize = internalGridSize;
			m_lines.reserve(gridSize*gridSize * 2 * 3);
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
				m_material = new graphics::Material(shader);
				m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				m_vertexBuffer = utils::D3d::CreateDynamicBufferFromVector(m_lines, D3D11_BIND_VERTEX_BUFFER);
			}
			
		}

		void EditorGrid::Draw(math::Vector3 cameraPos)
		{
			UINT stride = sizeof(LineVertex);
			UINT offset = 0;
			if (m_material)
			{
				int scale = (int)log10(((abs(cameraPos.y/2)+1) / m_cellSize)*m_cellSize);
				scale = pow(10.0f,scale);
				math::Matrix worldMatrix = math::Matrix::CreateScale((scale)*m_cellSize) * math::Matrix::CreateTranslation(
					(int)(cameraPos.x / scale)*scale,
					0.0f,
					(int)(cameraPos.z / scale)*scale
				);
				scale *= m_cellSize;
				math::Vector4 cameraScaleMatrix(cameraPos.x, cameraPos.y, cameraPos.z, 0);
				m_material->SetMatrix("thomas_ObjectToWorld", worldMatrix.Transpose());
				m_material->SetVector("cameraPos", math::Vector4(cameraScaleMatrix));
				m_material->SetInt("gridScale", scale);
				m_material->GetShader()->BindVertexBuffer(m_vertexBuffer, stride, offset);
				m_material->Bind();
				m_material->Draw(m_lines.size(), 0);
			}
			
		}

		EditorGrid::~EditorGrid()
		{
			SAFE_RELEASE(m_vertexBuffer);
		}

	}
}
