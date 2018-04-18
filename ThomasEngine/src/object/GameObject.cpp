
#include "GameObject.h"
#include "../ThomasManaged.h"
void ThomasEditor::GameObject::Destroy()
{
	Monitor::Enter(Scene::CurrentScene->GetGameObjectsLock());
	Monitor::Enter(m_componentsLock);
	for (int i = 0; i < m_components.Count; i++) {
		m_components[i]->Destroy();
		i--;
	}
	thomas::object::Object::Destroy(nativePtr);
	m_components.Clear();
	Monitor::Exit(m_componentsLock);

	Scene::CurrentScene->GameObjects->Remove(this);
	ThomasWrapper::SelectedGameObjects->Remove(this);
	Monitor::Exit(Scene::CurrentScene->GetGameObjectsLock());
}