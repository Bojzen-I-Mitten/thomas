#pragma once

#include <map>
#include "../../include/AntTweakBar.h"
#include "Math.h"

namespace thomas
{
	namespace utils
	{
		class DebugTools
		{
		public:
			struct Bar {
				TwBar *bar;
				bool visible;
			};
		private:
			
			static void __stdcall ReloadShadersButtonCallback(void* clientData);
			static void __stdcall CopyStdStringToClient(std::string& destClientString, const std::string& sourceLibraryString);
		public:
			static void Init();
			static void Destroy();
			static void RemoveAllVariables(std::string barName = "mainBar");
			static void SetPosition(int x, int y);
			static void AddFloat(float &variable, const char* name, std::string barName = "mainBar");
			static void AddFloatWithStep(float &variable, const char* name, const char *input, std::string barName = "mainBar");
			static void AddInteger(int &variable, const char* name, std::string barName = "mainBar");
			static void AddIntegerWithStep(int &variable, const char* name, const char *input, std::string barName = "mainBar");
			static void AddBool(bool &variable, const char* name, std::string barName = "mainBar");
			static void AddColor(math::Color  &color, const char* name, std::string barName = "mainBar");
			static void AddDirectionVector(math::Vector3& vector, const char* name, std::string barName = "mainBar");
			static void AddQuaternion(math::Quaternion& quaternion, const char* name, std::string barName = "mainBar");
			static void AddRotation(math::Quaternion& rotation, const char* name, std::string barName = "mainBar");
			static void AddString(std::string& string, const char* name, std::string barName = "mainBar");
			static void AddEnum(TwType enumDesc, int& enumData, const char * name, std::string barName = "mainBar");
			static void Draw();
			static void ToggleVisibility(std::string barName = "mainBar");
			static void Hide(std::string barName = "mainBar");
			static void Show(std::string barName = "mainBar");
			static int ProcessMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			static Bar* GetBar(std::string name);
			static void CreateBar(std::string name);
			static void RemoveBar(std::string name);
		private:

			static std::map<std::string, Bar> s_bars;

		};

	}
}