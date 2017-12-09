#include "Transform.h"
#include "../GameObject.h"
namespace thomas
{
	namespace object
	{
		namespace component
		{

			void Transform::Decompose() {
				m_localWorldMatrix.Decompose(m_localScale, m_localRotation, m_localPosition);
			}

			Transform::Transform()
			{
				Decompose();
				m_parent = NULL;
			}
			math::Vector3 Transform::Forward()
			{
				return GetWorldMatrix().Forward();
			}
			math::Vector3 Transform::Up()
			{
				return GetWorldMatrix().Up();
			}
			math::Vector3 Transform::Right()
			{
				return GetWorldMatrix().Right();
			}

			math::Matrix Transform::GetLocalWorldMatrix()
			{
				return m_localWorldMatrix;
			}

			math::Matrix Transform::GetWorldMatrix()
			{

				if (m_parent)
					return m_localWorldMatrix * m_parent->GetWorldMatrix();
				else
					return m_localWorldMatrix;
			}

			void Transform::LookAt(Transform * target)
			{
				if (target->GetPosition() == GetPosition())
					return;
				math::Matrix lookAt = math::Matrix::CreateLookAt(GetPosition(), target->GetPosition(), m_localWorldMatrix.Up());

				lookAt = lookAt.Invert();

				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(m_localPosition, lookAt.Forward(), lookAt.Up());

				Decompose();

			}
			void Transform::LookAt(math::Vector3 target)
			{
				if (target == GetPosition())
					return;
				math::Matrix lookAt = math::Matrix::CreateLookAt(GetPosition(), target, m_localWorldMatrix.Up());

				lookAt = lookAt.Invert();

				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(m_localPosition, lookAt.Forward(), lookAt.Up());

				Decompose();

			}
			void Transform::Rotate(math::Vector3 angles)
			{
				math::Quaternion rot = math::Quaternion::CreateFromYawPitchRoll(angles.x, angles.y, angles.z);
				math::Matrix newRot = math::Matrix::Transform(math::Matrix::CreateFromQuaternion(rot), m_localRotation);
				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(m_localPosition, newRot.Forward(), newRot.Up());

				Decompose();
			}
			void Transform::Rotate(float x, float y, float z)
			{
				return Rotate(math::Vector3(x, y, z));
			}
			void Transform::RotateByAxis(math::Vector3 axis, float angle)
			{
				math::Quaternion rot = math::Quaternion::CreateFromAxisAngle(axis, angle);
				math::Matrix newRot = math::Matrix::CreateFromQuaternion(m_localRotation * rot);
				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(m_localPosition, newRot.Forward(), newRot.Up());
				Decompose();

			}
			void Transform::Translate(math::Vector3 translation)
			{
				math::Matrix pos = math::Matrix::CreateTranslation(translation);

				m_localWorldMatrix *= pos;
				Decompose();
			}
			void Transform::Translate(float x, float y, float z)
			{
				return Translate(math::Vector3(x, y, z));
			}

			math::Vector3 Transform::GetPosition()
			{
				if (m_parent)
					return m_parent->GetPosition() + math::Vector3::Transform(m_localPosition, math::Matrix::CreateFromQuaternion(m_parent->GetRotation()));
				else
					return m_localPosition;
			}
			math::Quaternion Transform::GetRotation()
			{
				if (m_parent)
					return m_localRotation*m_parent->GetRotation();
				else
					return m_localRotation;
			}
			math::Vector3 Transform::GetEulerAngles()
			{
				if (m_parent)
					return m_eulerAngles * m_parent->GetEulerAngles();
				else
					return m_eulerAngles;
			}
			math::Vector3 Transform::GetScale()
			{
				if (m_parent)
					return m_parent->GetScale()*m_localScale;
				else
					return m_localScale;
			}
			void Transform::SetPosition(math::Vector3 position)
			{

				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(position, m_localWorldMatrix.Forward(), m_localWorldMatrix.Up());
				Decompose();
			}
			void Transform::SetPosition(float x, float y, float z)
			{
				SetPosition(math::Vector3(x, y, z));
			}
			void Transform::SetRotation(math::Quaternion rotation)
			{

				math::Matrix rotMatrix = math::Matrix::CreateFromQuaternion(rotation);

				m_localWorldMatrix = math::Matrix::CreateScale(m_localScale) * math::Matrix::CreateWorld(m_localPosition, rotMatrix.Forward(), rotMatrix.Up());
				Decompose();
				m_eulerAngles = math::ToEuler(GetRotation());
			}
			void Transform::SetRotation(float yaw, float pitch, float roll)
			{
				SetRotation(math::Quaternion::CreateFromYawPitchRoll(math::DegreesToRadians(yaw), math::DegreesToRadians(pitch), math::DegreesToRadians(roll)));
				m_eulerAngles = math::Vector3(pitch, yaw, roll);
			}
			void Transform::SetScale(math::Vector3 scale)
			{
				//Bug here
				m_localWorldMatrix = math::Matrix::CreateScale(scale) * math::Matrix::CreateWorld(m_localPosition, m_localWorldMatrix.Forward(), m_localWorldMatrix.Up());
				Decompose();
				m_localScale = scale;
			}
			void Transform::SetScale(float x, float y, float z)
			{
				return SetScale(math::Vector3(x, y, z));
			}
			void Transform::SetScale(float scale)
			{
				return SetScale(math::Vector3(scale, scale, scale));
			}

			void Transform::SetParent(Transform * parent)
			{
				if (m_parent != parent)
				{
					RemoveParent();
					m_parent = parent;
					m_parent->m_children.push_back(this);
				}
			}
			Transform * Transform::GetParent()
			{
				return m_parent;
			}
			void Transform::RemoveParent()
			{
				if (m_parent) //Remove from old parent
				{
					for (int i = 0; i < m_parent->m_children.size(); i++)
					{
						if (m_parent->m_children[i] == this)
						{
							m_parent->m_children[i] = NULL;
							m_parent->m_children.erase(m_parent->m_children.begin() + i);
							i -= 1;
						}
							
					}
				}
				m_parent = NULL;
			}
			void Transform::OnDestroy()
			{	
				for (int i = 0; i < m_children.size(); i++)
				{
					GameObject::Destroy(m_children[i]->m_gameObject);
					m_children.erase(m_children.begin() + i);
					i -= 1;
				}
			}
		}
	}
}
