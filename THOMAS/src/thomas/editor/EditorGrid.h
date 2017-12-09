#pragma once
#include "../Common.h"
#include "../utils/Math.h"
namespace thomas
{
	namespace graphics
	{
		class Material;
	}
	namespace editor
	{
		class THOMAS_API EditorGrid
		{
		private:
			void AddLine(math::Vector3 from, math::Vector3 to, math::Vector4 color);
		public:
			EditorGrid(int gridSize, float cellSize, int internalGridSize);
			EditorGrid(int gridSize, float cellSize) : EditorGrid(gridSize, cellSize, 0) {};
			EditorGrid(int gridSize) : EditorGrid(gridSize, 1.0f, 0) {}
			void Draw(math::Vector3 cameraPos);
			~EditorGrid();
		private:
			struct LineVertex
			{
				math::Vector3 pos;
				math::Vector4 color;
			};
			std::vector<LineVertex> m_lines;
			graphics::Material* m_material = nullptr;
			math::Matrix worldMatrix;
			ID3D11Buffer* m_vertexBuffer;
			int m_gridSize;
			float m_cellSize;
			int m_internalGridSize;
		};
	}
}