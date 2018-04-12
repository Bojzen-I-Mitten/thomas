#pragma once
#include "../Common.h"

#include "../utils/Math.h"
#include <vector>
#include <map>
namespace thomas {

	class Scene;
	namespace object { class GameObject; namespace component 
	{ 
		class Camera;
		class Transform;
		class RenderComponent;
	} }

	namespace resource
	{
		class Material;
	}

	namespace graphics
	{
				
		class Mesh;
		struct RenderCommand
		{
			object::component::Camera* camera;
			object::component::Transform* transform;
			Mesh* mesh;
			resource::Material* material;

			RenderCommand(object::component::Transform* trans, Mesh* m, resource::Material* mat, object::component::Camera* cam) : 
				transform(trans), mesh(m), material(mat), camera(cam) {};
		};
		typedef std::map<object::component::Camera*, std::map<resource::Material*, std::vector<RenderCommand>>> CommandQueue;

		class THOMAS_API Renderer {
		private:			
			static void BindFrame();
			static void BindObject(thomas::resource::Material* material, thomas::object::component::Transform* transform);
			
			
		public:
			static void BindCamera(thomas::object::component::Camera* camera);

			static void ProcessCommands();
			static void ClearCommands();

			static void SubmitCommand(RenderCommand command);
		private:
			static CommandQueue s_renderCommands;
			static CommandQueue s_lastFramesCommands;
		};
	}
}