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
			static void RenderQueue(std::vector<RenderPair*> renderQueue);

			static bool SortPairs(RenderPair* a, RenderPair* b);
			
			static void BindFrame();
			
		public:
			static void Begin();
			static void Render();
			static void BindObject(thomas::graphics::Material* material, thomas::object::component::Transform* transform);
			static void BindCamera(thomas::object::component::Camera* camera);
			static void ClearRenderQueue();

			static void AddToRenderQueue(graphics::RenderPair* renderPair);
			static std::vector<graphics::RenderPair*> GetRenderQueue();
		private:
			
			
			static std::vector<graphics::RenderPair*> s_renderQueue;
			static std::vector<graphics::RenderPair*> s_lastFramesRenederQueue;
		};
	}
}