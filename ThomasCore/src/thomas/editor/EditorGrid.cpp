#include "EditorGrid.h"
#include "..\resource\Material.h"
#include "..\resource\Shader.h"
#include "..\object\component\Camera.h"
#include "..\graphics\Renderer.h"

namespace thomas
{
	namespace editor
	{
		EditorGrid::EditorGrid(const int & gridSize, const float & cellSize, const int & internalGridSize) : m_gridSize(gridSize), m_cellSize(cellSize),
																											 m_internalGridSize(internalGridSize)
		{
			CreateGrid();
		}

		void EditorGrid::Draw(object::component::Camera* camera)
		{
			//Check if the material is available and send properties
			if (m_material)
			{
				math::Vector3 cameraPos = camera->GetPosition();
				int scale = (int)log10(((abs(cameraPos.y / 2.f) + 1.f) / m_cellSize) * m_cellSize);
				scale = (int)pow(10.0f, scale);
				math::Matrix worldMatrix = math::Matrix::CreateScale((scale) * m_cellSize) * math::Matrix::CreateTranslation(
											(int)(cameraPos.x / (float)scale) * (float)scale, 0.f, (int)(cameraPos.z / (float)scale) * (float)scale);

				scale *= (int)m_cellSize;
				math::Vector4 cameraScaleMatrix(cameraPos.x, cameraPos.y, cameraPos.z, 0.f);
				m_material->SetVector("cameraPos", math::Vector4(cameraScaleMatrix));
				m_material->SetInt("gridScale", scale);
				graphics::Renderer::SubmitCommand(graphics::RenderCommand(worldMatrix, m_mesh, m_material.get(), camera));
			}
		}

		void EditorGrid::CreateGrid()
		{
			//Grid colors
			const math::Vector4 darkGray(0.40625f, 0.40625f, 0.40625f, 1.0f);
			const math::Vector4 lightGray(0.3046875f, 0.3046875f, 0.3046875f, 0.2f);

			for (int i = -m_gridSize / 2; i <= m_gridSize / 2; i += (int)m_cellSize)
			{
				//Add a line
				math::Vector3 from((float)i, 0.f, (float)(-m_gridSize / 2));
				math::Vector3 to((float)i, 0.f, (float)(m_gridSize / 2));
				AddLine(from, to, darkGray);

				//Add a line
				from = math::Vector3((float)-m_gridSize / 2.f, 0.f, (float)i);
				to = math::Vector3((float)m_gridSize / 2.f, 0.f, (float)i);
				AddLine(from, to, darkGray);

				for (float j = m_cellSize / m_internalGridSize; j < m_cellSize; j += m_cellSize / m_internalGridSize)
				{
					//Add a line
					from = math::Vector3((float)i + j, 0.f, (float)(-m_gridSize / 2));
					to = math::Vector3((float)i + j, 0.f, (float)(m_gridSize / 2));
					AddLine(from, to, lightGray, 0.6f);

					//Add a line
					from = math::Vector3((float)(-m_gridSize / 2), 0.f, (float)i + j);
					to = math::Vector3((float)(m_gridSize / 2), 0.f, (float)i + j);
					AddLine(from, to, lightGray, 0.6f);
				}
			}

			resource::Shader* shader = resource::Shader::CreateShader("../Data/FXIncludes/EditorGridShader.fx"); //Hardcoded grid shader
			if (shader != nullptr)
			{
				//Prepare the material
				m_material = std::make_unique<resource::Material>(shader);
				m_material->m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				m_mesh = std::shared_ptr<graphics::Mesh>(new graphics::Mesh(m_lines, {}, "grid"));
				m_lines.positions.clear();
				m_lines.colors.clear();
			}
		}

		void EditorGrid::AddLine(const math::Vector3 & from, const math::Vector3 & to, const math::Vector4 & color, const float & viewDistance)
		{
			m_lines.positions.push_back(math::Vector4(from.x, from.y, from.z, viewDistance));
			m_lines.colors.push_back(color);
	
			m_lines.positions.push_back(math::Vector4(to.x, to.y, to.z, viewDistance));
			m_lines.colors.push_back(color);
		}
	}
}
