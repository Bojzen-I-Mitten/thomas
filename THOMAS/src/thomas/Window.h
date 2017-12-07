#pragma once
#include <Windows.h>
#include "Common.h"
namespace thomas 
{

	class THOMAS_API Window
	{
	private:
		
		bool UpdateWindow();
		bool InitDxBuffers();

		Window(HINSTANCE hInstance, int nCmdShow, LONG width, LONG height, LPWSTR title);
		Window(HWND hWnd);
	public:
		enum class Ratio
		{
			STANDARD_169 = 0,
			STANDARD_1610 = 1,
			STANDARD_43 = 2,
		};

		struct DXBuffers
		{
			ID3D11RenderTargetView* backBuffer;
			ID3D11ShaderResourceView* backBufferSRV;
			ID3D11DepthStencilState* depthStencilState;
			ID3D11DepthStencilView* depthStencilView;
			ID3D11DepthStencilView* depthStencilViewReadOnly;
			ID3D11ShaderResourceView* depthBufferSRV;
		}m_dxBuffers;

	public:

		static LRESULT CALLBACK EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static void InitEditor(HWND hWnd);
		static Window* Create(HWND hWnd);
		static Window* GetEditorWindow();
		static Window* GetWindow(int index);
		static Window* GetWindow(HWND hWnd);
		static std::vector<Window*> GetWindows();
		static void Destroy();

		static void ClearAllWindows();
		static void PresentAllWindows();

		void Bind();
		void Present();

		~Window();

		IDXGISwapChain* GetSwapChain();
		void Clear();

		bool SetHeight(LONG height);
		bool SetWidth(LONG width);
		bool SetAspectRatio();
		bool SetSize(LONG height, LONG width);
		bool Resize();

		LONG GetHeight();
		LONG GetWidth();
		Ratio GetAspectRatio();
		float GetRealAspectRatio();
		HWND GetWindowHandler();
		LONG GetHorizontalResolution();
		LONG GetVerticalResolution();
		
		bool Initialized();

		void SetCursor(bool visible);
		

		bool ChangeWindowShowState(int nCmdShow);

		void SetFullScreen(bool fullSceeen);
	private:
		LONG m_width;
		LONG m_height;
		float m_aspectRatio;
		WNDCLASSEX m_windowClassInfo;
		HWND m_windowHandler;
		RECT m_windowRectangle;
		LPWSTR m_title;
		bool m_showCursor;
		Ratio m_ratio;
		bool m_fullScreen;
		bool m_initialized;

		IDXGISwapChain* m_swapChain;

		static std::vector<Window*> s_windows;
		static Window* s_editorWindow;
		static Window* s_current;

	private:

	};
}


