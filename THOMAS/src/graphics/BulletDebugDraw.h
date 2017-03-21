#pragma once
#define _XM_NO_INTRINSICS_ 1;
#include "../../include/LinearMath/btIDebugDraw.h"
#include "DirectXTK/PrimitiveBatch.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/Effects.h"
#include "DirectXTK/CommonStates.h"
#include "../utils/Math.h"
#include "../Common.h"
#include "../object/component/Camera.h"
namespace thomas
{
	namespace graphics
	{

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
			struct CameraStruct
			{
				math::Matrix viewProjection;
			}m_cameraData;
			ID3D11InputLayout* m_inputLayout;
			ID3D11VertexShader* m_vertexShader;
			ID3D11Buffer* m_lineBuffer;
			ID3D11Buffer* m_cameraBuffer;
			ID3D11PixelShader* m_pixelShader;
			int m_debugMode;
			std::vector<LineVertex> m_lines;

		};
	}
}
