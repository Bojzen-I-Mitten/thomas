#include "HeightMap.h"
#include "noise\noise.h"
#include "../graphics/Mesh.h"
namespace thomas
{
	namespace utils
	{

		std::vector<float> HeightMap::s_height;

		void HeightMap::ApplyHeightMap(int size, float detail, float mapSize, Plane::PlaneData& plane, math::Vector2 startingPos)
		{

			float width = size * detail;
			float height = width;
			float e = 0.0f;

			noise::module::Perlin myModule;

			myModule.SetNoiseQuality(noise::NoiseQuality::QUALITY_BEST);

			myModule.SetFrequency(1.f);

			myModule.SetSeed(rand() % 1000);

			float x, y;
			for (int i = 0; i < plane.verts.size(); i++)
			{
				x = plane.verts[i].position.x * detail;
				y = -(plane.verts[i].position.z * detail);

				e = 0.0f;
				float nx = x / width - 0.5f;
				float ny = y / height - 0.5f;


				e += myModule.GetValue(nx, ny, 0) / 2.0f + 0.5f;
				e += myModule.GetValue(2 * nx, 2 * ny, 0) / 2.0f + 0.5f;
				e += myModule.GetValue(4 * nx, 4 * ny, 0) / 2.0f + 0.5f;
				e += myModule.GetValue(8 * nx, 8 * ny, 0) / 2.0f + 0.5f;
				e += myModule.GetValue(16 * nx, 16 * ny, 0) / 2.0f + 0.5f;
				e += myModule.GetValue(32 * nx, 32 * ny, 0) / 2.0f + 0.5f;
				e = pow(e, 3.0f);


				float d = 2 * sqrt(nx*nx + ny*ny);
				e = (e + 1.00) * (1 - 2.00*pow(d, 1.70));

				if (e < 0.0f)
					e = -55.0f;

				plane.verts[i].position.y = e;
			}
			CalculateNormals(size, detail, plane);
		}

		void HeightMap::CalculateNormals(int size, float detail, Plane::PlaneData & plane)
		{
			int width = size * detail;
			int height = width;

			math::Vector3 bottomLeft, upperLeft, upperRight;
			math::Vector3 vec1, vec2;

			for (int y = 0; y < height - 1; y++)
			{
				for (int x = 0; x < width - 1; x++)
				{
					int index1 = ((y + 1) * width) + x;
					int index2 = ((y + 1) * width) + (x + 1);
					int index3 = ((y * width) + x);

					bottomLeft = plane.verts[index1].position;
					upperLeft = plane.verts[index2].position;
					upperRight = plane.verts[index3].position;

					vec1 = bottomLeft - upperLeft;
					vec2 = bottomLeft - upperRight;

					int index = (y * width) + x;

					plane.verts[index].normal = vec1.Cross(vec2);
					float lenght = (plane.verts[index].normal.Length());
					plane.verts[index].normal = plane.verts[index].normal * (1 / lenght);
				}
			}

			/*math::Vector3 sum;
			int index = 0;
			float length;*/
			//// now sum that shit up
			/*for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					sum = math::Vector3(0.0f);
					if (((x - 1) >= 0) && ((y - 1) >= 0))
					{
						index = ((x - 1) * (width - 1)) + (x - 1);
						sum = sum + plane.verts[index].normal;
					}

					if ((x < (width - 1)) && ((y - 1) >= 0))
					{
						index = ((y - 1) * (width - 1)) + x;
						sum = sum + plane.verts[index].normal;
					}

					if (((x - 1) >= 0) && (y < (height - 1)))
					{
						index = (y * (width - 1)) + (x - 1);
						sum = sum + plane.verts[index].normal;
					}

					if ((x < (width - 1)) && (y < (height - 1)))
					{
						index = (y * (width - 1)) + x;
						sum = sum + plane.verts[index].normal;
					}

					length = sum.Length();
					index = (y * width) + x;

					plane.verts[index].normal = sum / length;
				}
			}*/

		}

		std::vector<float> HeightMap::GetHeight()
		{
			return s_height;
		}
	}
}