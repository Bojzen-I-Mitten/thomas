#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>

namespace thomas {

	class Scene;
	namespace object { class GameObject; namespace component 
	{ 
		class Camera;
		class Transform;
		class RenderComponent;
	} }

	namespace graphics
	{
		class Mesh;
		class Material;
		struct RenderPair
		{
			object::component::Transform* transform;
			Mesh* mesh;
			Material* material;
		};

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
						
			static void BindPerFrame();
			static void BindPerCamera(thomas::object::component::Camera* camera);
			static void BindPerObject(thomas::graphics::Material* material, thomas::object::component::Transform* transform);
			

			static void Render(Scene* scene);
			static void RenderCamera(thomas::object::component::Camera* camera);

			static void BeginRender();
			static void EndRender();

			static void RenderQueue(std::vector<RenderPair*> renderQueue);

			static bool SortPairs(RenderPair* a, RenderPair* b);

			static void BindDepthNormal();
			static void BindDepthReadOnly();

			static void BindDepthBufferTexture();
			static void UnbindDepthBufferTexture();

			static void ResetDepthStencilState();

		private:
			static ID3D11RenderTargetView* s_backBuffer;
			static ID3D11ShaderResourceView* s_backBufferSRV;
			static ID3D11DepthStencilState* s_depthStencilState;
			static ID3D11DepthStencilView* s_depthStencilView;
			static ID3D11DepthStencilView* s_depthStencilViewReadOnly;
			static ID3D11ShaderResourceView* s_depthBufferSRV;
		};
	}
}