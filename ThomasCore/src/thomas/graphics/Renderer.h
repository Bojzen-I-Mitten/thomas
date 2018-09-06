/*
	Render class for the gameobjects and material assembly
*/

#pragma once
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

		struct MaterialSorter
		{
			bool operator() (resource::Material* mat1, resource::Material* mat2) const;
		};

		typedef std::map<object::component::Camera*, std::map<resource::Material*, std::vector<RenderCommand>, MaterialSorter>> CommandQueue;

		class Renderer {
		private:

			static void BindFrame();
			static void BindObject(thomas::resource::Material* material, const thomas::math::Matrix& worldMatrix);
						
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