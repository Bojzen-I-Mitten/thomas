#include "AssimpLoader.h"

#include "../graphics/Texture.h"
#include "../graphics/Mesh.h"
#include "../resource/Material.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>


namespace thomas
{
	namespace utils
	{
		std::vector<graphics::Mesh*> AssimpLoader::LoadModel(std::string path)
		{

			std::vector<graphics::Mesh*> meshes;
			std::string dir = path.substr(0, path.find_last_of("\\/"));
			// Read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile
			(
				path,
				aiProcess_FindDegenerates |
				aiProcess_FindInvalidData |
				aiProcess_ImproveCacheLocality |
				aiProcess_JoinIdenticalVertices |
				aiProcess_OptimizeGraph |
				aiProcess_OptimizeMeshes |
				aiProcess_RemoveRedundantMaterials |
				aiProcess_SortByPType |
				aiProcess_Triangulate |
				aiProcess_RemoveComponent |
				aiProcess_ValidateDataStructure |
				aiProcess_GenSmoothNormals |
				aiProcess_CalcTangentSpace |
				aiProcess_FlipUVs |
				aiProcess_PreTransformVertices
			);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				LOG("ERROR::ASSIMP " << importer.GetErrorString());
				return meshes;
			}


			// Process ASSIMP's root node recursively
			ProcessNode(scene->mRootNode, scene, meshes);
			return meshes;
		}

		std::string AssimpLoader::GetMaterialName(aiMaterial * material)
		{
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			return std::string(name.C_Str());
		}

		int AssimpLoader::GetMaterialShadingModel(aiMaterial * material)
		{
			int shadingModel;
			material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
			return shadingModel;
		}

		math::Color AssimpLoader::GetMaterialColor(aiMaterial * material, const char * pKey, unsigned int type, unsigned int idx)
		{
			aiColor3D color;
			material->Get(pKey, type, idx, color);
			return math::Color(color.r, color.g, color.b);
		}

		float AssimpLoader::GetMaterialShininess(aiMaterial * material)
		{
			float shininess;
			material->Get(AI_MATKEY_SHININESS, shininess);
			return shininess;
		}

		float AssimpLoader::GetMaterialShininessStrength(aiMaterial * material)
		{
			float shininessStrength;
			material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
			return shininessStrength;
		}

		int AssimpLoader::GetMaterialBlendMode(aiMaterial * material)
		{
			int blendMode;
			material->Get(AI_MATKEY_BLEND_FUNC, blendMode);
			return blendMode;
		}

		std::vector<graphics::Texture*> AssimpLoader::GetMaterialTextures(aiMaterial * material)
		{
			struct TextureInfo {
				aiString textureNameAiString;
				graphics::Texture::TextureType textureType;
				int mappingMode;
			};
			std::vector<TextureInfo> textureInfos;


			//Get all textures information


			TextureInfo texInfo;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texInfo.textureNameAiString) == AI_SUCCESS)
			{
				texInfo.textureType = graphics::Texture::TextureType::DIFFUSE;
				material->Get(_AI_MATKEY_MAPPING_BASE, aiTextureType_DIFFUSE, 0, texInfo.mappingMode);
				textureInfos.push_back(texInfo);
			}
				
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &texInfo.textureNameAiString) == AI_SUCCESS)
			{
				texInfo.textureType = graphics::Texture::TextureType::SPECULAR;
				material->Get(_AI_MATKEY_MAPPING_BASE, aiTextureType_SPECULAR, 0, texInfo.mappingMode);
				textureInfos.push_back(texInfo);
			}
				
			if (material->GetTexture(aiTextureType_NORMALS, 0, &texInfo.textureNameAiString) == AI_SUCCESS)
			{
				texInfo.textureType = graphics::Texture::TextureType::NORMAL;
				material->Get(_AI_MATKEY_MAPPING_BASE, aiTextureType_NORMALS, 0, texInfo.mappingMode);
				textureInfos.push_back(texInfo);
			}
			else if (material->GetTexture(aiTextureType_HEIGHT, 0, &texInfo.textureNameAiString) == AI_SUCCESS)
			{
				texInfo.textureType = graphics::Texture::TextureType::NORMAL;
				material->Get(_AI_MATKEY_MAPPING_BASE, aiTextureType_NORMALS, 0, texInfo.mappingMode);
				textureInfos.push_back(texInfo);
			}

				

			//Create a texture object for every texture found.
			std::vector<graphics::Texture*> textures;
			//for (unsigned int i = 0; i < textureInfos.size(); i++)
			//{
			//	std::string textureName(textureInfos[i].textureNameAiString.C_Str());
			//	graphics::Texture* texture = graphics::Texture::CreateTexture
			//	(
			//		textureInfos[i].mappingMode,
			//		textureInfos[i].textureType,
			//		dir + "/" + textureName
			//	);
			//	if(texture->Initialized())
			//		textures.push_back(texture);
			//}
			return textures;
			
		}

		float AssimpLoader::GetMaterialOpacity(aiMaterial * material)
		{
			float opacity;
			material->Get(AI_MATKEY_OPACITY, opacity);
			return opacity;
		}

		graphics::Mesh* AssimpLoader::ProcessMesh(aiMesh * mesh, const aiScene* scene, std::string meshName)
		{
			graphics::Vertices vertices;
			std::vector <int> indices;
			std::string name = meshName + "-" + std::string(mesh->mName.C_Str());
			resource::Material* material;

			//vector<Texture> textures;
			vertices.positions.resize(mesh->mNumVertices);
			vertices.uvs.resize(mesh->mNumVertices);
			vertices.normals.resize(mesh->mNumVertices);
			vertices.tangents.resize(mesh->mNumVertices);
			vertices.bitangents.resize(mesh->mNumVertices);

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				

				// Positions
				vertices.positions[i].x = mesh->mVertices[i].x;
				vertices.positions[i].y = mesh->mVertices[i].y;
				vertices.positions[i].z = mesh->mVertices[i].z;

				// Normals
				vertices.normals[i].x = mesh->mNormals[i].x;
				vertices.normals[i].y = mesh->mNormals[i].y;
				vertices.normals[i].z = mesh->mNormals[i].z;

				// Tangents
				if (mesh->HasTangentsAndBitangents())
				{
					vertices.tangents[i].x = mesh->mTangents[i].x;
					vertices.tangents[i].y = mesh->mTangents[i].y;
					vertices.tangents[i].z = mesh->mTangents[i].z;

					// Bitangents
					vertices.bitangents[i].x = mesh->mBitangents[i].x;
					vertices.bitangents[i].y = mesh->mBitangents[i].y;
					vertices.bitangents[i].z = mesh->mBitangents[i].z;

				}
				

				// Texture Coordinates
				if (mesh->mTextureCoords[0])
				{
					math::Vector2 vec;

					// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vertices.uvs[i].x = mesh->mTextureCoords[0][i].x;
					vertices.uvs[i].y = mesh->mTextureCoords[0][i].y;
				}
			}

			// Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
			
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			//Process materials

			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			//TODO: material import
			//material = graphics::Material::CreateMaterial(dir, materialType, mat);


			graphics::Mesh* m = new graphics::Mesh(vertices, indices, name);
			return m;
		}

		void AssimpLoader::ProcessNode(aiNode * node, const aiScene * scene, std::vector<graphics::Mesh*> &meshes)
		{
			std::string modelName(scene->mRootNode->mName.C_Str());
			std::string nodeName(node->mName.C_Str());
			if (nodeName == modelName)
				nodeName = "";
			// Process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// The node object only contains indices to index the actual objects in the scene. 
				// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				graphics::Mesh* processedMesh = ProcessMesh(mesh, scene, modelName + "-" + nodeName + "-" + std::to_string(i));
				meshes.push_back(processedMesh);
			}
			// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene, meshes);
			}
		}
	}
}
