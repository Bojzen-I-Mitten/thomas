#include "Resource.h"
#include <Windows.h>
#include "Shlwapi.h"
namespace thomas
{
	namespace resource
	{
		Resource::Resource(std::string path)
		{
			m_path = path;
		}

		std::string Resource::GetName()
		{
			std::string name = PathFindFileName(m_path.c_str());
			name = name.substr(0, name.find_last_of('.'));
			return name;	
			
		}

		std::string Resource::GetPath()
		{
			return m_path;
		}

		std::string Resource::PathToName(std::string path)
		{
			std::string name = PathFindFileName(path.c_str());
			name = name.substr(0, name.find_last_of('.'));
			return name;
		}
	}
}
