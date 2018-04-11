#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "../graphics/Buffers.h"
namespace thomas
{
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
			void Draw(math::Vector3 cameraPos);
			~EditorGrid();
		private:
			struct LineVertices
			{
				std::vector<math::Vector4> positions;
				std::vector<math::Vector4> colors;
				
			};
			LineVertices m_lines;
			resource::Material* m_material = nullptr;
			math::Matrix worldMatrix;
			std::vector<graphics::buffers::VertexBuffer*> m_vertexBuffers;
			int m_gridSize;
			float m_cellSize;
			int m_internalGridSize;
		};
	}
}