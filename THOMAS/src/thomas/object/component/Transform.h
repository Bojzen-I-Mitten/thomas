#pragma once
#include "Component.h"
#include "../../utils/Math.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{
			class THOMAS_API Transform: public Component
			{
			private:
				void Decompose();
			public:
				
				math::Quaternion m_localRotation;
				math::Vector3 m_localPosition;
				math::Vector3 m_localScale;
				
				math::Matrix m_localWorldMatrix;

				
				std::vector<Transform*> m_children;

				Transform();
				
				math::Vector3 Forward();
				math::Vector3 Up();
				math::Vector3 Right();

				math::Matrix GetLocalWorldMatrix();
				math::Matrix GetWorldMatrix();

				void LookAt(Transform* target);
				void LookAt(math::Vector3 target);
				void Rotate(math::Vector3 angles);
				void Rotate(float x, float y, float z);
				void RotateByAxis(math::Vector3 axis, float angle);
				void Translate(math::Vector3 translation);
				void Translate(float x, float y, float z);


				math::Vector3 GetPosition();
				math::Quaternion GetRotation();
				math::Vector3 GetScale();
				
				void SetPosition(math::Vector3 position);
				void SetPosition(float x, float y, float z);
				void SetRotation(math::Quaternion rotation);
				void SetRotation(float yaw, float pitch, float roll);
				void SetScale(math::Vector3 scale);
				void SetScale(float x, float y, float z);
				void SetScale(float scale);

				void UpdateChildren();
				void SetParent(Transform* parent);
				Transform* GetParent();
				void RemoveParent();
				void OnDestroy();

			private:
				Transform* m_parent;
				
			};
		}
	}
}