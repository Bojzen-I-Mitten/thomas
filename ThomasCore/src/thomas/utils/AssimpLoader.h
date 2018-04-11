#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Math.h"


struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
namespace thomas
{
	namespace graphics 
	{
		class Mesh;
		class Texture;
	}
	namespace utils
	{

		class AssimpLoader
		{
		private:
			static graphics::Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string meshName);
			static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<graphics::Mesh*> &meshes);
		public:

			static std::vector<graphics::Mesh*> LoadModel(std::string path);
			
			static std::string GetMaterialName(aiMaterial* material);
			static int GetMaterialShadingModel(aiMaterial* material);
			static math::Color GetMaterialColor(aiMaterial* material, const char* pKey, unsigned int type, unsigned int idx);
			static float GetMaterialShininess(aiMaterial* material);
			static float GetMaterialShininessStrength(aiMaterial* material);
			static int GetMaterialBlendMode(aiMaterial* material);
			static float GetMaterialOpacity(aiMaterial* material);

			static std::vector<graphics::Texture*> GetMaterialTextures(aiMaterial* material);

		private:
			
		};

	}
}

