#include "Resource.h"

namespace thomas
{
	namespace resource
	{
		Resource::Resource(std::string path)
		{
			m_path = path;
		}

		thomas::resource::Resource::~Resource()
		{
		}
	}
}
