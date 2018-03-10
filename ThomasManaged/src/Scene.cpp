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
