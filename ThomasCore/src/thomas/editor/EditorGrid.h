/*
	Class for management of the grid in the scene.
*/

#pragma once
#include "..\utils\Math.h"
#include "..\graphics\Mesh.h"
#include <memory>

namespace thomas
{
	namespace utils { namespace buffers { class VertexBuffer; } }
	namespace object { namespace component { class Camera; } }
	namespace resource { class Material; }

	namespace editor
	{
		class THOMAS_API EditorGrid
		{
		public:
			EditorGrid(const int & gridSize, const float & cellSize = 1.0f, const int & internalGridSize = 0);
			~EditorGrid() = default;
			void Draw(object::component::Camera* camera);

		private:
			void CreateGrid();
			void AddLine(const math::Vector3 & from, const math::Vector3 & to, const math::Vector4 & color, const float & viewDistance = 1.0f);
			
		private:
			graphics::Vertices m_lines;
			std::shared_ptr<graphics::Mesh> m_mesh; //Could probs be unique ptr too
			std::unique_ptr<resource::Material> m_material;
			math::Matrix worldMatrix;

		private:
			int m_gridSize;
			int m_internalGridSize;
			float m_cellSize;
		};
	}
}