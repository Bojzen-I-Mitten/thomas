/*
	Render class for the gameobjects and material assembly
*/

#pragma once
#include "..\Common.h"
#include "..\utils\Math.h"
#include <vector>
#include <map>
#include <memory>

namespace thomas 
{
	class Scene;
	namespace object
	{
		class GameObject;
		namespace component
		{
			class Camera;
			class Transform;
			class RenderComponent;
		}
	}

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
			std::shared_ptr<Mesh> mesh; //This doesn't have to be a shared ptr? More process overhead!
			resource::Material* material;

			RenderCommand(math::Matrix world, std::shared_ptr<Mesh> m, resource::Material* mat, object::component::Camera* cam) :
				worldMatrix(world), mesh(m), material(mat), camera(cam) {};
		};

		//This could probably be simplified...
		using CommandQueue = std::map<object::component::Camera*, std::map<resource::Material*, std::vector<RenderCommand>>>;

		class THOMAS_API Renderer 
		{						
		public:
			static void BindCamera(thomas::object::component::Camera* camera);
			static void ProcessCommands();
			static void ClearCommands();
			static void SubmitCommand(RenderCommand command);
			static void TransferCommandList();

		private:
			static void BindFrame();
			static void BindObject(thomas::resource::Material* material, const thomas::math::Matrix& worldMatrix);

		private:
			static CommandQueue s_renderCommands;
			static CommandQueue s_lastFramesCommands;
		};
	}
}