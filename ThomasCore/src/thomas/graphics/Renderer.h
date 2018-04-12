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
			math::Matrix worldMatrix;
			Mesh* mesh;
			resource::Material* material;

			RenderCommand(math::Matrix world, Mesh* m, resource::Material* mat, object::component::Camera* cam) : 
				worldMatrix(world), mesh(m), material(mat), camera(cam) {};
		};
		typedef std::map<object::component::Camera*, std::map<resource::Material*, std::vector<RenderCommand>>> CommandQueue;

		class THOMAS_API Renderer {
		private:			
			static void BindFrame();
			static void BindObject(thomas::resource::Material* material, thomas::math::Matrix& worldMatrix);
						
		public:
			static void BindCamera(thomas::object::component::Camera* camera);

			static void ProcessCommands();
			static void ClearCommands();

			static void SubmitCommand(RenderCommand command);

			static void TransferCommandList();
		private:
			static CommandQueue s_renderCommands;
			static CommandQueue s_lastFramesCommands;
		};
	}
}