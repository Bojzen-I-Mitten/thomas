#pragma once
#include "Math.h"
namespace thomas
{
	namespace utils
	{
		struct Ray
		{
			math::Vector3 direction;
			math::Vector3 origin;
			Ray(math::Vector3 origin, math::Vector3 direction) : origin(origin), direction(direction){}
			math::Vector3 GetPoint(float distance) { return origin + direction*distance; }
		};

		struct Bounds
		{
			math::Vector3 center;
			math::Vector3 extends;
			math::Vector3 GetMax() { return center + extends; }
			math::Vector3 GetMin() { return center - extends; }
			math::Vector3 GetSize() { return extends*2.0f; }

			Bounds(math::Vector3 center, math::Vector3 size) : center(center), extends(size*0.5f) {}
			bool Contains(math::Vector3 point) 
			{
				math::Vector3 min = GetMin();
				math::Vector3 max = GetMax();
				return (point.x > min.x && point.x < max.x &&
						point.y > min.y && point.y < max.y &&
						point.z > min.z && point.z < max.z);
			}
		};
	}
}