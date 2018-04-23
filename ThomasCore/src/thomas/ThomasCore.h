/*
	Assembly class of the engine (creates, updates and destroys objects).
*/

#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include <imgui\imgui.h>

namespace thomas
{
	class ThomasCore
	{
	public:	
		static bool Init();
		static bool Initialized();
		static void Update();
		static bool Destroy();
		static void Exit();

	public:
		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();
		static std::vector<std::string> GetLogOutput();

	public:
		static void LogOutput(const std::string & message);
		static void ClearLogOutput();

	private:
		static bool InitDirectX();
		static bool CreateDeviceAndContext();

	private:
		static bool s_initialized;
		static std::vector<std::string> s_logOutput;
		static bool s_clearLog;

	private:
		static ID3D11Device* s_device;
		static ID3D11DeviceContext* s_context;
		static ID3D11Debug* s_debug;
		static ImGuiContext* s_imGuiContext;
	};
}