#include "Scene.h"
#include "object\GameObject.h"
void ThomasEditor::Scene::Play()
{
		m_playing = true;
		for each(GameObject^ gObj in m_gameObjects)
		{
			gObj->Awake();
		}
}

void ThomasEditor::Scene::UnLoad()
{
	for each(GameObject^ gObj in m_gameObjects)
	{
		gObj->Destroy();
	}
	m_gameObjects.Clear();
}

void ThomasEditor::Scene::PostLoad()
{
	for each(GameObject^ gObj in m_gameObjects)
	{
		gObj->PostLoad();
	}
}
