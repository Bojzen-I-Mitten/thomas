#pragma once
#include <map>
#include "Shader.h"
#include "Texture.h"
#include "../utils/AssimpLoader.h"

namespace thomas
{
	namespace graphics
	{
		class Material
		{
		private:
			virtual Material* CreateInstance(std::string dir, std::string name, aiMaterial* assimpMaterial, Shader* shader) { return NULL; }
			virtual Material* CreateInstance(std::string name, Shader* shader) { return NULL; }
		public:
			Material(Shader* shader);
			Material(std::string shader);
			Material(std::string name, Shader* shader);
			Material(std::string name, std::string shader);

			static Material* CreateMaterial(Material* material);
			static Material* CreateMaterial(std::string dir, std::string materialType, aiMaterial* assimpMaterial);
			static Material* CreateMaterial(std::string name, std::string materialType);
			//static bool RegisterNewMaterialType(std::string type, Material* material);
			static Material* RegisterNewMaterialType(std::string type, Material* material);

			static Material* GetMaterialByName(std::string name);

			static std::vector<Material*> GetLoadedMaterials();
			static std::vector<Material*> GetMaterialsByShader(Shader* shader);
			static std::vector<Material*> GetMaterialsByShader(std::string name);

			static void Destroy();
			static void Destroy(Shader* shader);

			bool Bind();
			bool Unbind();

			virtual void Update() {};

			std::string GetName();
			Shader* GetShader();

			virtual ~Material();

		private:

			static std::vector<Material*> s_materials;
			static std::map<std::string, Material*> s_materialTypes;

		protected:
			D3D11_PRIMITIVE_TOPOLOGY m_shaderTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			Shader* m_shader;
			std::string m_materialName;
			ID3D11Buffer* m_materialPropertiesBuffer;
			std::vector<Texture*> m_textures;
		};
	}
}

