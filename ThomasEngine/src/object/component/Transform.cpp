#include "Transform.h"
#include "../GameObject.h"
void ThomasEditor::Transform::OnDestroy()
{
	if (parent)
	{
		parent = nullptr;
	}
	for (int i = 0; i < children->Count; i++)
	{
		children[i]->gameObject->Destroy();
		//m_children.erase(m_children.begin() + i);
		i -= 1;
	}
}