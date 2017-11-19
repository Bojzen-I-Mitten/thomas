#pragma once
#include "Scene.h"
#include "object\GameObject.h"



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
	TestScene() : Scene("TestScene")
	{
		LoadObject<TestCamera>();
	}
};
