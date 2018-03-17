#pragma once
#include <string>
namespace thomas
{
	namespace resource
	{
		class Resource
		{
		public:
			Resource(std::string name);
			~Resource();
		private:
			std::string m_name;
		};
	}
}