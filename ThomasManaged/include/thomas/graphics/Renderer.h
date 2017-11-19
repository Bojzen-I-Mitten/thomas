#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>

namespace thomas {

	namespace object { class GameObject; namespace component { class Camera; } }

	namespace graphics
	{
		class THOMAS_API Renderer {
		private:
			
			//static void BindGameObjectBuffer(object::component::Camera* camera, object::GameObject* gameObject);
		public:

			
			static bool Init();
			static bool Resize();

			static void Clear();
			static void Render();

			static bool Destroy();

			static std::vector<object::component::Camera*> GetCameras();
			
			static ID3D11ShaderResourceView* GetDepthBufferSRV();
			static ID3D11RenderTargetView* GetBackBuffer();

			
			static void UpdateGameObjectBuffer(object::component::Camera* camera, object::GameObject* gameObject);
			static void BindGameObjectBuffer();
			static void UnBindGameObjectBuffer();
			static void RenderSetup(object::component::Camera* camera);

			static void BindDepthNormal();
			static void BindDepthReadOnly();

			static void BindDepthBufferTexture();
			static void UnbindDepthBufferTexture();

			static void ResetDepthStencilState();

		private:
			struct GameObjectBuffer
			{
				math::Matrix worldMatrix;
				math::Matrix viewMatrix;
				math::Matrix projectionMatrix;
				math::Matrix mvpMatrix;
				math::Vector3 camPos;
				float buffer;
				math::Vector3 camDir;
				float buffer2;
			};
			static ID3D11RenderTargetView* s_backBuffer;
			static ID3D11ShaderResourceView* s_backBufferSRV;
			static ID3D11RasterizerState* s_rasterState;
			static ID3D11RasterizerState* s_wireframeRasterState;
			static ID3D11DepthStencilState* s_depthStencilState;
			static ID3D11DepthStencilView* s_depthStencilView;
			static ID3D11DepthStencilView* s_depthStencilViewReadOnly;
			static ID3D11ShaderResourceView* s_depthBufferSRV;

			static ID3D11Buffer* s_objectBuffer;
			static GameObjectBuffer s_objectBufferStruct;
		};
	}
}