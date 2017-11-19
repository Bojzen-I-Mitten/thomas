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
				
				Component(std::string name);
				bool GetActive();
			private:
			protected:
				
			};
		}
	}
}