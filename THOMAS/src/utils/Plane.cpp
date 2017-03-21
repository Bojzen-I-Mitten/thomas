#include "Plane.h"
#include <cmath>
#include <algorithm>
namespace thomas
{
	namespace utils
	{
		Plane::PlaneData Plane::CreatePlane(int size, float detail)
		{
			std::vector<graphics::Vertex> verts;
			std::vector<int> indices;
			float width = (float)size * detail;
			float height = width;

			//float x, y, z;
			graphics::Vertex temp_vert;

			// Defualt temp values, must be calulated later
			temp_vert.normal.x = 0.0f;
			temp_vert.normal.y = 1.0f;
			temp_vert.normal.z = 0.0f;
			temp_vert.bitangent.x = 0.0f;
			temp_vert.bitangent.y = 0.0f;
			temp_vert.bitangent.z = -1.0f;
			temp_vert.tangent.x = 1.0f;
			temp_vert.tangent.y = 0.0f;
			temp_vert.tangent.z = 0.0f;

			temp_vert.tangent.x = 0.0f;
			temp_vert.tangent.y = 0.0f;
			temp_vert.tangent.z = -1.0f;

			temp_vert.bitangent.x = 1.0f;
			temp_vert.bitangent.y = 0.0f;
			temp_vert.bitangent.z = 0.0f;

			// Create points
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					temp_vert.position.y = 0.0f;
					temp_vert.position.x = (float)y / detail;
					temp_vert.position.z = -(float)x / detail;
					temp_vert.uv.x = (y / ((float)size * detail));
					temp_vert.uv.y = (x / ((float)size * detail));
					verts.push_back(temp_vert);
				}
			}

			// triangulate points
			for (int i = 0; i < (size * detail) - 1; i++)
			{
				int index = size*detail * (i + 1);
				for (int j = 0; j < (size * detail) - 1; j++)
				{

					indices.push_back(j + i * size*detail);
					indices.push_back(index);
					indices.push_back(index + 1);

					indices.push_back(j + i * size*detail);
					indices.push_back(index + 1);
					indices.push_back((j + i * size*detail) + 1);

					index++;
				}
			}

			Plane::PlaneData plane;
			plane.verts = verts;
			plane.indices = indices;
			return plane;
		}
	}
}