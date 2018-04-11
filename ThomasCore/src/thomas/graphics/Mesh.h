#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>

namespace thomas 
{
	namespace resource
	{
		class Shader;
	}
	namespace graphics 
	{
		
		struct Vertex 
		{
			math::Vector3 position;
			math::Vector2 uv;
			math::Vector3 normal;
			math::Vector3 tangent;
			math::Vector3 bitangent;
		};

		struct MeshData
		{
			std::vector<Vertex> vertices;
			std::vector<int> indices;
			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;
		};

		class THOMAS_API Mesh
		{
		private:
			void SetupMesh();
			math::BoundingBox GenerateBounds();
		public:
			Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::string name);
			~Mesh();
			bool SetName(std::string name);

			MeshData* GetData();
	
			std::string GetName();

			int GetIndexCount();
			int GetVertexCount();

			void Draw(resource::Shader* shader);
			
			std::vector<Vertex>* GetVertices();
			std::vector<int>* GetIndices();

			math::BoundingBox m_bounds;
		private:
			std::string m_name;
			MeshData m_data;
		};
	}
}

