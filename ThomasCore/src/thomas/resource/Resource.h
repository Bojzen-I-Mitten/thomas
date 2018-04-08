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
			std::string GetPath();
			~Resource();
		protected:
			std::string m_path;
		};
	}
}