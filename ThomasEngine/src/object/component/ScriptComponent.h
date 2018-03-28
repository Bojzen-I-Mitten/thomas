#pragma once
#pragma unmanaged
#include <thomas\object\component\ScriptComponent.h>
#pragma managed


#include "../Component.h"

namespace ThomasEditor
{
	public ref class ScriptComponent : public Component
	{
	private:

	public:
		virtual void Start()override {};
		virtual void Update()override {};
		virtual void OnDrawGizmosSelected() override {};
		virtual void OnDrawGizmos() override {};

		ScriptComponent() : Component(new thomas::object::component::ScriptComponent())
		{
		}
	};
}