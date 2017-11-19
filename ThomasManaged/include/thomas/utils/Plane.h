#pragma once

#include "../Common.h"
#include <vector>
namespace thomas
{
	namespace graphics { struct Vertex; }
	namespace utils
	{

		class THOMAS_API Plane
		{
		public:
			struct PlaneData
			{
				std::vector<graphics::Vertex> verts;
				std::vector<int> indices;
			};

		private:
			Plane();

		public:


			static PlaneData Plane::CreatePlane(int size, float detail);

		private:
		};
	}
}