#pragma once
#include "..\utils\Math.h"
#include "..\resource\Shader.h"
#include <vector>
#include <map>
#include <memory>

namespace thomas
{
	namespace graphics
	{
		struct Vertices
		{
			std::vector<math::Vector4> positions;
			std::vector<math::Vector4> colors;
			std::vector<math::Vector2> texCoord0;
			std::vector<math::Vector3> normals;
			std::vector<math::Vector3> tangents;
			std::vector<math::Vector3> bitangents;
		};

		struct MeshData
		{
			Vertices vertices;
			std::vector<unsigned int> indices;
			std::map<resource::Shader::Semantics, std::unique_ptr<utils::buffers::VertexBuffer>> vertexBuffers;
			std::unique_ptr<utils::buffers::IndexBuffer> indexBuffer;
		};

		class THOMAS_API Mesh
		{
		public:
			Mesh(const Vertices & vertices, std::vector<unsigned int> indices, const std::string & name);
			~Mesh() = default;
			void Draw(resource::Shader* shader);

		public:
			void SetName(const std::string & name);

		public:
			std::string GetName() const;
		    unsigned int GetIndexCount() const;
			unsigned int GetVertexCount() const;
			Vertices & GetVertices();
			std::vector<unsigned int> & GetIndices();
			MeshData & GetData();

		private:
			void SetupMesh();
			math::BoundingBox GenerateBounds();

		public:
			math::BoundingBox m_bounds; //This should be private

		private:
			std::string m_name;
			MeshData m_data;
		};
	}
}

