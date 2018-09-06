/*
	Helper file for loading models with the assimp library. This class prepares the mesh
	with vertex and index buffers and renders the model.
*/

#pragma once
#include "..\utils\Math.h"
#include "..\resource\Shader.h"
#include <vector>
#include <map>
#include <memory>

namespace thomas
{
	namespace object
	{
		namespace component
		{
			class Camera;
			class Transform;
		}
	}
	namespace resource { class Material; }


	namespace graphics 
	{
		struct BoneWeight
		{
			int boneIndex0 = 0;
			int boneIndex1 = 0;
			int boneIndex2 = 0;
			int boneIndex3 = 0;

			float weight0 = 0.0f;
			float weight1 = 0.0f;
			float weight2 = 0.0f;
			float weight3 = 0.0f;

			void AddBoneData(int boneIndex, float weight)
			{
				if (weight0 == 0.0f)
				{
					boneIndex0 = boneIndex;
					weight0 = weight;
				}
				else if (weight1 == 0.0f)
				{
					boneIndex1 = boneIndex;
					weight1 = weight;
				}
				else if (weight2 == 0.0f)
				{
					boneIndex2 = boneIndex;
					weight2 = weight;
				}
				else if (weight3 == 0.0f)
				{
					boneIndex3 = boneIndex;
					weight3 = weight;
				}
			}
			
		};
		struct Vertices 
		{
			std::vector<math::Vector4> positions;
			std::vector<math::Vector4> colors;
			std::vector<math::Vector2> texCoord0;
			std::vector<math::Vector3> normals;
			std::vector<math::Vector3> tangents;
			std::vector<math::Vector3> bitangents;
			std::vector<BoneWeight> boneWeights;
		};

		struct MeshData
		{
			Vertices vertices;
			std::vector<unsigned int> indices;
			
			std::map<resource::Shader::Semantics, std::unique_ptr<utils::buffers::VertexBuffer>> vertexBuffers;
			std::unique_ptr<utils::buffers::IndexBuffer> indexBuffer = nullptr;
		};

		class Mesh
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
			math::BoundingBox m_bounds;

		private:
			std::string m_name;
			MeshData m_data;

			
		};
	}
}

