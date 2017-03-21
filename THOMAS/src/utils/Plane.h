#pragma once

#include "../graphics/Model.h"
#include "../Common.h"


namespace thomas
{
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