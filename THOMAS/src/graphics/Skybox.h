#pragma once
#include "../utils/Math.h"
#include "Shader.h"
#include "Texture.h"

namespace thomas
{
	namespace graphics
	{
		class Skybox
		{
		private:
			void SetupBuffers();
			void CreateRasterizer();
			void CreateDepthStencilState();
		public:
			Skybox(std::string path, std::string shaderName, int slot);
			~Skybox();
			void LoadCubeMap(std::string path, int slot);
			bool Bind(math::Matrix viewMatrix, math::Matrix mvpMatrix);
			bool BindCubemap();
			bool Unbind();
			void Draw();
			void BindSkyboxTexture();
			void SetLerp(math::Vector3 lerp);

		private:
			ID3D11RasterizerState* m_rasterizerP;
			ID3D11DepthStencilState* m_depthstencilP;
			UINT m_depthRefP;

			struct CubeData
			{
				ID3D11Buffer* vertexBuffer;
				ID3D11Buffer* indexBuffer;
				ID3D11Buffer* constantBuffer;
				ID3D11RasterizerState* rasterizerState;
				ID3D11DepthStencilState* depthStencilState;
				std::vector<Texture*> texture;
				Shader* shader;
			};



			struct mvpStruct
			{
				math::Matrix mvpMatrix;
				math::Matrix viewMatrix;
				math::Vector3 camPosition;
				float paddeo;
				math::Vector3 lerp;
				float padding;
			};

			std::vector <math::Vector3> vertices =
			{
				// Front Face
				math::Vector3(-1.0f, -1.0f, -1.0f),
				math::Vector3(-1.0f,  1.0f, -1.0f),
				math::Vector3(1.0f,  1.0f, -1.0f),
				math::Vector3(1.0f, -1.0f, -1.0f),

				// Back Face
				math::Vector3(-1.0f, -1.0f, 1.0f),
				math::Vector3(1.0f, -1.0f, 1.0f),
				math::Vector3(1.0f,  1.0f, 1.0f),
				math::Vector3(-1.0f,  1.0f, 1.0f),

				// Top Face
				math::Vector3(-1.0f, 1.0f, -1.0f),
				math::Vector3(-1.0f, 1.0f,  1.0f),
				math::Vector3(1.0f, 1.0f,  1.0f),
				math::Vector3(1.0f, 1.0f, -1.0f),

				// Bottom Face
				math::Vector3(-1.0f, -1.0f, -1.0f),
				math::Vector3(1.0f, -1.0f, -1.0f),
				math::Vector3(1.0f, -1.0f,  1.0f),
				math::Vector3(-1.0f, -1.0f,  1.0f),

				// Left Face
				math::Vector3(-1.0f, -1.0f,  1.0f),
				math::Vector3(-1.0f,  1.0f,  1.0f),
				math::Vector3(-1.0f,  1.0f, -1.0f),
				math::Vector3(-1.0f, -1.0f, -1.0f),

				// Right Face
				math::Vector3(1.0f, -1.0f, -1.0f),
				math::Vector3(1.0f,  1.0f, -1.0f),
				math::Vector3(1.0f,  1.0f,  1.0f),
				math::Vector3(1.0f, -1.0f,  1.0f),
			};

			std::vector<int> indices = {
				// Front Face
				0,  1,  2,
				0,  2,  3,

				// Back Face
				4,  5,  6,
				4,  6,  7,

				// Top Face
				8,  9, 10,
				8, 10, 11,

				// Bottom Face
				12, 13, 14,
				12, 14, 15,

				// Left Face
				16, 17, 18,
				16, 18, 19,

				// Right Face
				20, 21, 22,
				20, 22, 23
			};

			CubeData m_data;
			mvpStruct m_mvpStruct;
		};
	}
}

