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

		struct BoundingSphere
		{
			math::Vector3 position;
			float radius;
			float intersection(Ray ray)
			{
				math::Vector3 p = ray.origin - position;
				float rSquared = radius*radius;
				float p_d = p.Dot(ray.direction);
				if (p_d > 0 || p.Dot(p) < rSquared)
					return 0; //No collision
				
				math::Vector3 a = p - p_d * ray.direction;
				float aSquared = a.Dot(a);

				if (aSquared > rSquared)
					return 0; //No collision

				float h = sqrtf(rSquared - aSquared);

				math::Vector3 i = a - h * ray.direction;

				math::Vector3 intersection = position + i;
				return math::Vector3::Distance(intersection, ray.origin);
			}
			BoundingSphere(math::Vector3 position, float radius) : position(position), radius(radius){}
		};

		struct Bounds
		{
			math::Vector3 center;
			math::Vector3 extends;
			math::Vector3 GetMax() { return center + extends; }
			math::Vector3 GetMin() { return center - extends; }
			math::Vector3 GetSize() { return extends*2.0f; }

			Bounds(math::Vector3 center, math::Vector3 size) : center(center), extends(size*0.5f) {}

			float Intersection(Ray ray)
			{
				math::Vector3 min = GetMin();
				math::Vector3 max = GetMax();
				math::Vector3 dirfrac(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);

				float t1 = (min.x - ray.origin.x)*dirfrac.x;
				float t2 = (max.x - ray.origin.x)*dirfrac.x;
				float t3 = (min.y - ray.origin.y)*dirfrac.y;
				float t4 = (max.y - ray.origin.y)*dirfrac.y;
				float t5 = (min.z - ray.origin.z)*dirfrac.z;
				float t6 = (max.z - ray.origin.z)*dirfrac.z;

				float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
				float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

				// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
				if (tmax < 0)
				{
					return 0;
				}

				// if tmin > tmax, ray doesn't intersect AABB
				if (tmin > tmax)
				{
					return 0;
				}

				return tmin;

			}

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