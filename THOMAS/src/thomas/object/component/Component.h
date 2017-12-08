#pragma once
#include "../../Common.h"
#include "../Object.h"
namespace thomas
{
	namespace object
	{
		class GameObject;
		namespace component
		{
			class THOMAS_API Component : public Object
			{
			private:
			public:
				GameObject* m_gameObject;
				virtual void Update() {};
			private:
			protected:
				
			};
		}
	}
}