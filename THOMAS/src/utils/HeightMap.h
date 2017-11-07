#pragma once
#include "../Common.h"
#include "Math.h"
#include <vector>
#include "Plane.h"
namespace thomas
{
	
	namespace utils
	{
		class HeightMap
		{
		public:
			static void HeightMap::ApplyHeightMap(int size, float detail, float mapSize, Plane::PlaneData& plane, math::Vector2 startingPos);
			static void HeightMap::CalculateNormals(int size, float detail, Plane::PlaneData & plane);
			static std::vector<float> GetHeight();
		private:
			static std::vector<float> s_height;
		};
	}
}