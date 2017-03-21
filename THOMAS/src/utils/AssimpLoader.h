#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Math.h"
#include "../../include/assimp/Importer.hpp"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/postprocess.h"

namespace thomas
{
	namespace graphics 
	{
		class Mesh;
		class Model;
		class Texture;
	}
	namespace utils
	{

		class AssimpLoader
		{
		private:
			static graphics::Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string meshName, std::string dir, std::string materialType);
			static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<graphics::Mesh*> &meshes, std::string dir, std::string materialType);
		public:
			static graphics::Model* LoadModel(std::string name, std::string path, std::string materialType);
			
			static std::string GetMaterialName(aiMaterial* material);
			static int GetMaterialShadingModel(aiMaterial* material);
			static math::Color GetMaterialColor(aiMaterial* material, const char* pKey, unsigned int type, unsigned int idx);
			static float GetMaterialShininess(aiMaterial* material);
			static float GetMaterialShininessStrength(aiMaterial* material);
			static int GetMaterialBlendMode(aiMaterial* material);
			static float GetMaterialOpacity(aiMaterial* material);

			static std::vector<graphics::Texture*> GetMaterialTextures(aiMaterial* material, std::string dir);

		private:
			
		};

	}
}

