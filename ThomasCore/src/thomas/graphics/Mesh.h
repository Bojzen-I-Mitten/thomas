#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>
#include <map>
#include "../resource/Shader.h"
namespace thomas 
{

	namespace graphics 
	{
		
		struct Vertices 
		{
			std::vector<math::Vector3> positions;
			std::vector<math::Vector2> uvs;
			std::vector<math::Vector3> normals;
			std::vector<math::Vector3> tangents;
			std::vector<math::Vector3> bitangents;
		};


		struct MeshData
		{
			Vertices vertices;
			std::vector<int> indices;
			std::map<resource::Shader::Semantics, utils::buffers::VertexBuffer*> vertexBuffers;
			utils::buffers::IndexBuffer* indexBuffer;
		};

		class THOMAS_API Mesh
		{
		private:
			void SetupMesh();
			math::BoundingBox GenerateBounds();
		public:
			Mesh(Vertices vertices, std::vector<int> indices, std::string name);
			~Mesh();
			bool SetName(std::string name);

			MeshData* GetData();
	
			std::string GetName();

			int GetIndexCount();
			int GetVertexCount();

			void Draw(resource::Shader* shader);
			
			Vertices* GetVertices();
			std::vector<int>* GetIndices();

			math::BoundingBox m_bounds;
		private:
			std::string m_name;
			MeshData m_data;
		};
	}
}

