#pragma once
#include "../Common.h"
#include "Resource.h"

namespace thomas
{
	namespace resource
	{
		template<typename Type>
		class Resources
		{
		public:
			static void Destroy();
			static Type* Load(std::string path);
		private:
			static std::vector<Resource> s_resources;
		};

		template<typename Type>
		std::vector<Resource> Resources<Type>::s_resources;

		template<typename Type>
		inline void Resources<Type>::Destroy()
		{
			s_resources.clear();
		}

		template<typename Type>
		inline Type* Resources<Type>::Load(std::string path)
		{
			Type* res = new Type(path);
			s_resources.push_back(res);
			return res;
		}
	}
}