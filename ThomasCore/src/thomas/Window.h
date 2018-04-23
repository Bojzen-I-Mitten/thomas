/*
	Singleton window management class
*/

#pragma once
#include <Windows.h>
#include "Common.h"

namespace thomas 
{
	class THOMAS_API Window
	{
	public:
		struct DXBuffers
		{
			ID3D11RenderTargetView* backBuffer = nullptr;
			ID3D11ShaderResourceView* backBufferSRV = nullptr;
			ID3D11DepthStencilState* depthStencilState = nullptr;
			ID3D11DepthStencilView* depthStencilView = nullptr;
			ID3D11DepthStencilView* depthStencilViewReadOnly = nullptr;
			ID3D11ShaderResourceView* depthBufferSRV = nullptr;
		}m_dxBuffers;

	public:
		~Window();
		void QueueResize();
		void Bind();
		void Present();
		void Clear();
		bool IsFocused();
		bool Initialized();
		bool ChangeWindowShowState(int nCmdShow);

	public:
		static void Destroy();
		static void ClearAllWindows();
		static void PresentAllWindows();
		static void BeginFrame();
		static void EndFrame(bool copyGui);
		static void Update();
		static void UpdateFocus();

	public:
		bool SetHeight(const LONG & height);
		bool SetWidth(const LONG & width);
		bool SetSize(const LONG & height, const LONG & width);
		void SetCursor(const bool & visible);

	public:
		static bool WaitingForUpdate();
		static LRESULT CALLBACK EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static void InitEditor(HWND hWnd);
		static Window* Create(HWND hWnd);

	public:
		static Window* GetEditorWindow();
		static Window* GetWindow(int index);
		static Window* GetWindow(HWND hWnd);
		static Window* GetCurrentBound();
		static std::vector<Window*> GetWindows();

	public:
		LONG GetHeight() const;
		LONG GetWidth() const;
		LONG GetHorizontalResolution() const;
		LONG GetVerticalResolution() const;
		IDXGISwapChain* GetSwapChain() const;
		HWND GetWindowHandler() const;
		float GetRealAspectRatio() const;

	private:
		Window(HINSTANCE hInstance, int nCmdShow, const LONG & width, const LONG & height, const LPCSTR & title);
		Window(HWND hWnd);

	private:
		void CloneGUIData();
		void DeleteGUIData();
		bool UpdateWindow();
		bool InitDxBuffers();
		bool Resize();

	private:
		LONG m_width;
		LONG m_height;
		bool m_showCursor;
		bool m_fullScreen;
		bool m_initialized;
		bool m_shouldResize;
		bool m_created;
		bool m_focused;
		float m_aspectRatio;
		std::string m_title;

	private:
		WNDCLASSEX m_windowClassInfo;
		HWND m_windowHandler;
		RECT m_windowRectangle;
		IDXGISwapChain* m_swapChain;
		ImDrawData* m_guiData = nullptr;

	private:
		static std::vector<Window*> s_windows;
		static Window* s_editorWindow;
		static Window* s_current;
	};
}


