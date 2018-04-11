#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include <LinearMath\btIDebugDraw.h>



#include "../utils/Math.h"
#include "../Common.h"
#include <vector>
#include "../utils/Buffers.h"
namespace thomas
{
	namespace object { namespace component { class Camera; } }
	namespace resource { class Material; }
	namespace graphics
	{
		
		class THOMAS_API BulletDebugDraw : public btIDebugDraw
		{
		private:
		public:
			BulletDebugDraw();
			~BulletDebugDraw();
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

			void drawLineFinal();

			bool m_initialized;

		private:
			struct Lines
			{
				std::vector<math::Vector3> positions;
				std::vector<math::Vector3> colors;
			}m_lines;
			resource::Material* m_material;
			int m_debugMode;
			utils::buffers::VertexBuffer* m_vertexBufferPos;
			utils::buffers::VertexBuffer* m_vertexBufferColor;
			math::Matrix m_viewProjection;
		};
	}
}
