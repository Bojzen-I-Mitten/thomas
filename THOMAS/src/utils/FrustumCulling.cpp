#include "FrustumCulling.h"

thomas::utils::FrustumCulling::ClippingPlane thomas::utils::FrustumCulling::m_clippingPlanes[6];

void thomas::utils::FrustumCulling::GenerateClippingPlanes(object::component::Camera * camera)
{
	math::Matrix viewProjMatrix = camera->GetViewProjMatrix();

	m_clippingPlanes[0].n.x = viewProjMatrix._14 + viewProjMatrix._11;
	m_clippingPlanes[0].n.y = viewProjMatrix._24 + viewProjMatrix._21;
	m_clippingPlanes[0].n.z = viewProjMatrix._34 + viewProjMatrix._31;
	m_clippingPlanes[0].d = viewProjMatrix._44 + viewProjMatrix._41;

	m_clippingPlanes[1].n.x = viewProjMatrix._14 - viewProjMatrix._11;
	m_clippingPlanes[1].n.y = viewProjMatrix._24 - viewProjMatrix._21;
	m_clippingPlanes[1].n.z = viewProjMatrix._34 - viewProjMatrix._31;
	m_clippingPlanes[1].d = viewProjMatrix._44 - viewProjMatrix._41;

	m_clippingPlanes[2].n.x = viewProjMatrix._14 + viewProjMatrix._12;
	m_clippingPlanes[2].n.y = viewProjMatrix._24 + viewProjMatrix._22;
	m_clippingPlanes[2].n.z = viewProjMatrix._34 + viewProjMatrix._32;
	m_clippingPlanes[2].d = viewProjMatrix._44 + viewProjMatrix._42;

	m_clippingPlanes[3].n.x = viewProjMatrix._14 - viewProjMatrix._12;
	m_clippingPlanes[3].n.y = viewProjMatrix._24 - viewProjMatrix._22;
	m_clippingPlanes[3].n.z = viewProjMatrix._34 - viewProjMatrix._32;
	m_clippingPlanes[3].d = viewProjMatrix._44 - viewProjMatrix._42;

	m_clippingPlanes[4].n.x = viewProjMatrix._13;
	m_clippingPlanes[4].n.y = viewProjMatrix._23;
	m_clippingPlanes[4].n.z = viewProjMatrix._33;
	m_clippingPlanes[4].d = viewProjMatrix._43;

	m_clippingPlanes[5].n.x = viewProjMatrix._14 - viewProjMatrix._13;
	m_clippingPlanes[5].n.y = viewProjMatrix._24 - viewProjMatrix._23;
	m_clippingPlanes[5].n.z = viewProjMatrix._34 - viewProjMatrix._33;
	m_clippingPlanes[5].d = viewProjMatrix._44 - viewProjMatrix._43;

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_clippingPlanes[i].Normalize();
	}

	return;
}

bool thomas::utils::FrustumCulling::Cull(object::component::FrustumCullingComponent* frustumCullingComponent)
{
	//check if point is inside the generated planes.
	for (unsigned int i = 0; i < 6; ++i)
	{
		if (frustumCullingComponent->GetPosition().Dot(m_clippingPlanes[i].n) + m_clippingPlanes[i].d + frustumCullingComponent->GetRadius() <= 0.0f)
		{
			return false;
		}
	}

	return true;
}
