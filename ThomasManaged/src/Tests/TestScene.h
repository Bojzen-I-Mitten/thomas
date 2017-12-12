#pragma once
#pragma unmanaged
#include <thomas\Scene.h>
#include <thomas\object\GameObject.h>
#include <thomas\graphics\Material.h>
#include <thomas\graphics\Model.h>
#include <thomas\object\component\Components.h>
#include <thomas\utils\AssimpLoader.h>
#pragma managed

class TestScene : public thomas::Scene
{
public:
	TestScene() : Scene("testScene")
	{
		thomas::utils::AssimpLoader::LoadModel("testModel","../Data/box.obj", "poop");
	}
};
