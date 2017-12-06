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

			
			static void Render();
			static std::vector<object::component::Camera*> GetCameras();
			
						
			static void BindPerFrame();
			static void BindPerCamera(thomas::object::component::Camera* camera);
			static void BindPerObject(thomas::graphics::Material* material, thomas::object::component::Transform* transform);
			

			static void Render(Scene* scene);
			static void RenderCamera(thomas::object::component::Camera* camera);


			static void RenderQueue(std::vector<RenderPair*> renderQueue);

			static bool SortPairs(RenderPair* a, RenderPair* b);


		};
	}
}