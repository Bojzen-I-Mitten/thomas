#pragma once
#include "../Common.h"
#include "Math.h"
namespace thomas
{
	namespace object {
		namespace component {
			class Camera;
			class FrustumCullingComponent;
		}
	}
	namespace utils
	{
		class THOMAS_API FrustumCulling
		{
		private:
			struct ClippingPlane
			{
				void Normalize()
				{
					float s = 1 / n.Length();
					n.x *= s;
					n.y *= s;
					n.z *= s;
					d *= s;
				}
				math::Vector3 n;
				float d;
			};
		public:
			static void GenerateClippingPlanes(object::component::Camera* camera);
			static bool Cull(object::component::FrustumCullingComponent* frustumCullingComponent);
		private:
			static ClippingPlane m_clippingPlanes[6];
		public:

		
		};

	}
}