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

		static void Start();
		static bool Initialized();
		static bool Destroy();
		static void Exit();


		static void Update();
		static void Render();
		static void Stop();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();

		static void LogOutput(std::string message);
		static std::vector<std::string> GetLogOutput();
		static void ClearLogOutput();

	private:
		static bool InitDirectX();
		static bool s_initialized;
		static ID3D11Device* s_device;
		static ID3D11DeviceContext* s_context;
		static ImGuiContext* s_imGuiContext;
		static ID3D11Debug* s_debug;
		static std::vector<std::string> s_logOutput;
		static bool s_clearLog;

	};
}