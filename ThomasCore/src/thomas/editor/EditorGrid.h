#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "../graphics/Mesh.h"
#include <memory>
namespace thomas
{
	namespace utils
	{
		namespace buffers
		{
			class VertexBuffer;
		}
	}
	namespace object { namespace component { class Camera; } }
	namespace resource
	{
		class Material;
	}
	namespace editor
	{
		class THOMAS_API EditorGrid
		{
		private:
			void AddLine(math::Vector3 from, math::Vector3 to, math::Vector4 color, float viewDistance=1.0f);
		public:
			EditorGrid(int gridSize, float cellSize, int internalGridSize);
			EditorGrid(int gridSize, float cellSize) : EditorGrid(gridSize, cellSize, 0) {};
			EditorGrid(int gridSize) : EditorGrid(gridSize, 1.0f, 0) {}
			void Draw(object::component::Camera* camera);
			~EditorGrid();
		private:
			graphics::Vertices m_lines;
			std::shared_ptr<graphics::Mesh> m_mesh;
			resource::Material* m_material = nullptr;
			math::Matrix worldMatrix;
			int m_gridSize;
			float m_cellSize;
			int m_internalGridSize;
		};
	}
}