#include "DebugTools.h"
#include "../graphics/Shader.h"
#include "../Window.h"
#include "../ThomasCore.h"
namespace thomas
{
	namespace utils
	{
		std::map<std::string, DebugTools::Bar> DebugTools::s_bars;

		void TW_CALL DebugTools::ReloadShadersButtonCallback(void * clientData)
		{
			graphics::Shader::ReloadShaders();
		}

		void TW_CALL DebugTools::CopyStdStringToClient(std::string & destClientString, const std::string & sourceLibraryString)
		{
			destClientString = sourceLibraryString;
		}

		void DebugTools::Init()
		{
			TwInit(TW_DIRECT3D11, ThomasCore::GetDevice());
			TwCopyStdStringToClientFunc(CopyStdStringToClient);
			TwWindowSize(Window::GetWidth(), Window::GetHeight());

			CreateBar("mainBar");
			TwDefine("mainBar fontsize=3 iconifiable=false");
			Hide();

			RemoveAllVariables("mainBar");
		}

		void DebugTools::Destroy()
		{
			TwTerminate();
		}

		void DebugTools::RemoveAllVariables(std::string barName)
		{
			if (GetBar(barName))
			{
				TwRemoveAllVars(GetBar(barName)->bar);
				TwAddButton(GetBar(barName)->bar, "shaderReload", ReloadShadersButtonCallback, NULL, "label='Reload Shaders(F5)' key=F5");
			}
			
		}

		void DebugTools::SetPosition(int x, int y)
		{
			/*int barPos[2] = { x, y };
			TwSetParam(s_bar.bar, NULL, "position", TW_PARAM_INT32, 2, &barPos);*/
		}


		int DebugTools::ProcessMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			TwEventWin(hWnd, message, wParam, lParam);
			return 0;
		}

		DebugTools::Bar * DebugTools::GetBar(std::string name)
		{
			if (s_bars.find(name) != s_bars.end())
				return &s_bars[name];
			else
				return NULL;
		}

		void DebugTools::CreateBar(std::string name)
		{
			
			if (s_bars.find(name) == s_bars.end())
			{
				Bar bar;
				bar.visible = true;
				bar.bar = TwNewBar(name.c_str());

				s_bars[name] = bar;
			}

		}

		void DebugTools::RemoveBar(std::string name)
		{
			TwDeleteBar(GetBar(name)->bar);
		}


		void DebugTools::AddFloat(float &variable, const char* name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_FLOAT, &variable, "");
		}

		void DebugTools::AddFloatWithStep(float &variable, const char* name, const char * input, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_FLOAT, &variable, input);
		}

		void DebugTools::AddInteger(int &variable, const char* name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_INT16, &variable, "");
		}

		void DebugTools::AddIntegerWithStep(int &variable, const char* name, const char * input, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_INT16, &variable, input);
		}

		void DebugTools::AddBool(bool & variable, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_BOOL16, &variable, "true='Enabled' false='Disabled'");
		}

		void DebugTools::AddColor(math::Color & color, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_COLOR4F, &color, "coloralpha=true");
		}


		void DebugTools::AddDirectionVector(math::Vector3 & vector, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_DIR3F, &vector, "");
		}

		void DebugTools::AddQuaternion(math::Quaternion & quaternion, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_QUAT4F, &quaternion, "");
		}

		void DebugTools::AddRotation(math::Quaternion & rotation, const char * name, std::string barName)
		{
			AddQuaternion(rotation, name);
		}

		void DebugTools::AddString(std::string & string, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, TW_TYPE_STDSTRING, &string, "");
		}

		void DebugTools::AddEnum(TwType enumDesc, int& enumData, const char * name, std::string barName)
		{
			TwAddVarRW(GetBar(barName)->bar, name, enumDesc, &enumData, "");
		}


		void DebugTools::Draw()
		{
			TwDraw();
		}

		void DebugTools::ToggleVisibility(std::string barName)
		{
			if (GetBar(barName)->visible)
				Hide(barName);
			else
				Show(barName);
		}

		void DebugTools::Hide(std::string barName)
		{
			GetBar(barName)->visible = false;
			std::string s = barName + " visible=false";
			TwDefine(s.c_str());
		}

		void DebugTools::Show(std::string barName)
		{
			GetBar(barName)->visible = true;
			std::string s = barName + " visible=true";
			TwDefine(s.c_str());
		}

	}
}