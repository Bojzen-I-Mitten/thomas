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
			virtual ~Resource() {};
			std::string GetName();
			std::string GetPath();

			static std::string PathToName(std::string path);
		protected:
			std::string m_path;
		};
	}
}