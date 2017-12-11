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
		ScriptComponent() : Component(new thomas::object::component::ScriptComponent())
		{
		}
	};
}