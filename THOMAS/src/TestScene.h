#pragma once
#include "Scene.h"
#include "object\GameObject.h"
#include "graphics\Material.h"
class TestCamera : public thomas::object::GameObject
{
public:
	TestCamera() : GameObject("TestCamera")
	{

	}

	void Start()
	{
		AddComponent<thomas::object::component::Camera>();
	}
};

class TestScene : public thomas::Scene
{
public:
	TestScene() : Scene("testScene")
	{
		LoadObject<TestCamera>();

	}
};
