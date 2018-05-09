#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>
#include <map>
#include "../resource/Shader.h"
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
			std::vector<BoneWeight> boneWeight;
		};


		struct MeshData
		{
			Vertices vertices;
			std::vector<int> indices;
			std::map<resource::Shader::Semantics, utils::buffers::VertexBuffer*> vertexBuffers;
			utils::buffers::IndexBuffer* indexBuffer = nullptr;
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

