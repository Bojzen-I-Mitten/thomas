#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include <LinearMath\btIDebugDraw.h>



#include "../utils/Math.h"
#include "../Common.h"
#include <vector>

namespace thomas
{
	namespace object { namespace component { class Camera; } }

	namespace graphics
	{
		class Material;
		class THOMAS_API BulletDebugDraw : public btIDebugDraw
		{
		private:
		public:
			BulletDebugDraw();
			virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

			virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

			virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

			virtual void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);

			virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

			virtual void reportErrorWarning(const char* warningString);

			virtual void draw3dText(const btVector3& location, const char* textString);

			virtual void Update(object::component::Camera* camera);

			virtual void setDebugMode(int debugMode);

			virtual int getDebugMode() const { return m_debugMode; }

		private:
			struct LineVertex
			{
				math::Vector3 pos;
				math::Vector3 color;
			};
			struct Test
			{
				LineVertex lines[2];
			};
			Material* m_material;
			int m_debugMode;
			std::vector<LineVertex> m_lines;
			ID3D11Buffer* m_vertexBuffer;
			math::Matrix m_viewProjection;
		};
	}
}
