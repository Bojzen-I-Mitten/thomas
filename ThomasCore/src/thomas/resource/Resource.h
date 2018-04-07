#pragma once
#include <string>
namespace thomas
{
	namespace resource
	{
		
		class Resource
		{
		public:
			Resource(std::string path);
			std::string GetName();
			~Resource();
		private:
			std::string m_path;
		};
	}
}