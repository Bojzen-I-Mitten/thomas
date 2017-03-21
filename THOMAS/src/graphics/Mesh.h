#pragma once
#include "../Common.h"
#include <d3d11.h>
#include "../utils/Math.h"
#include <vector>
#include "../../include/assimp/Importer.hpp"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/postprocess.h"
#include "../ThomasCore.h"
#include "Material.h"

namespace thomas 
{
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
			
		public:
			Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::string name, Material* material);
			~Mesh();
			bool SetName(std::string name);

			MeshData* GetData();
	
			std::string GetName();

			int GetIndexCount();
			int GetVertexCount();
			
			std::vector<Vertex>* GetVertices();
			std::vector<int>* GetIndices();

			Material* GetMaterial();

			bool Bind();
			bool Unbind();

			void Draw();

		private:
			std::string m_name;
			MeshData m_data;
			Material* m_material;

		};
	}
}

