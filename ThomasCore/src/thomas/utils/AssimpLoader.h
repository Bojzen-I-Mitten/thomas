#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Math.h"
#include <memory>
#include "../resource/Model.h"
#include <assimp\matrix4x4.h>
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
			static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string meshName, resource::Model::ModelData& modelData, aiMatrix4x4& transform);
			static void ProcessSkeleton(aiNode* node, resource::Model::ModelData& modelData, int parentBone, math::Matrix globalInverseTransform, math::Matrix parentTransform);
			static void ProcessNode(aiNode* node, const aiScene* scene, resource::Model::ModelData& modelData);
		public:

			static resource::Model::ModelData LoadModel(std::string path);
			
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

