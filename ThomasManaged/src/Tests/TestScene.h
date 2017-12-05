#pragma once
#include <thomas\Scene.h>
#include <thomas\object\GameObject.h>
#include <thomas\graphics\Material.h>
#include <thomas\graphics\Model.h>
#include <thomas\object\component\Components.h>
#include <thomas\utils\AssimpLoader.h>

class TestScene : public thomas::Scene
{
public:
	TestScene() : Scene("testScene")
	{
		new thomas::graphics::Material("testMat", thomas::graphics::Shader::CreateShader("standardShader", "../Data/FXIncludes/ThomasStandardShader.fx"));
		thomas::utils::AssimpLoader::LoadModel("testModel","../Data/box.obj", "poop");
	}
};
