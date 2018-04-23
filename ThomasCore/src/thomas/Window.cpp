#include "Window.h"
#include "Input.h"
#include "ThomasCore.h"
#include "utils\d3d.h"
#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>
#include <imgui\ImGuizmo.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace thomas 
{
	std::vector<Window*> Window::s_windows;
	Window* Window::s_editorWindow = nullptr;
	Window* Window::s_current = nullptr;

	Window::Window(HINSTANCE hInstance, int nCmdShow, const LONG & width, const LONG & height, const LPCSTR & title) : m_focused(false), m_created(false), 
		m_shouldResize(false), m_width(width), m_height(height), m_title(std::string(title)), m_showCursor(false), m_fullScreen(false)
	{
		m_windowClassInfo = { 0 };
		m_windowClassInfo.cbSize = sizeof(WNDCLASSEX);
		m_windowClassInfo.style = CS_HREDRAW | CS_VREDRAW;
		m_windowClassInfo.lpfnWndProc = EventHandler; //Callback for EVENTS
		m_windowClassInfo.hInstance = hInstance;
		m_windowClassInfo.lpszClassName = "ThomasWindow";
		m_windowClassInfo.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));

		if (!RegisterClassEx(&m_windowClassInfo))
			LOG("Failed to create window")

		m_windowRectangle = { 0, 0, m_width, m_height };

		//Properties for window
		AdjustWindowRect(&m_windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);
		m_windowHandler = CreateWindow(
			"ThomasEngine",
			m_title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_windowRectangle.right - m_windowRectangle.left,
			m_windowRectangle.bottom - m_windowRectangle.top,
			nullptr,
			nullptr,
			hInstance,
			nullptr);

		m_initialized = false;
		if (m_windowHandler)
		{
			bool hr = utils::D3d::CreateSwapChain(m_width, m_height, m_windowHandler, m_swapChain, ThomasCore::GetDevice());
			if (hr)
			{
				m_created = true;
				ChangeWindowShowState(nCmdShow);
			}
		}
	}

	Window::Window(HWND hWnd) : m_focused(false), m_created(false), m_shouldResize(false), m_initialized(false)
	{
		bool result = GetWindowRect(hWnd, &m_windowRectangle);
		if (result)
		{
			m_height = m_windowRectangle.bottom;
			m_width = m_windowRectangle.right;
			m_showCursor = true; m_fullScreen = false;
			m_windowHandler = hWnd;

			if (utils::D3d::CreateSwapChain(m_width, m_height, m_windowHandler, m_swapChain, ThomasCore::GetDevice()))
				m_created = true;
		}
		else
			LOG("Failed to create window");
	}

	Window::~Window()
	{
		SAFE_RELEASE(m_dxBuffers.backBuffer);
		SAFE_RELEASE(m_dxBuffers.backBufferSRV);
		SAFE_RELEASE(m_dxBuffers.depthStencilState);
		SAFE_RELEASE(m_dxBuffers.depthStencilView);
		SAFE_RELEASE(m_dxBuffers.depthStencilViewReadOnly);
		SAFE_RELEASE(m_dxBuffers.depthBufferSRV);
		SAFE_RELEASE(m_swapChain);

		if (s_editorWindow = this)
			ImGui_ImplDX11_Shutdown();

		DestroyWindow(m_windowHandler);
	}

	void Window::InitEditor(HWND hWnd)
	{
		Window* window = new Window(hWnd);
		if (window->m_created)
		{
			s_editorWindow = window;
			ImGui_ImplDX11_Init(hWnd, ThomasCore::GetDevice(), ThomasCore::GetDeviceContext());
		}		
	}

	Window* Window::Create(HWND hWnd)
	{
		Window* window = new Window(hWnd);
		if (window->m_created)
		{
			s_windows.push_back(window);
			return window;
		}
		return nullptr;
	}

	bool Window::Resize()
	{
		bool result = GetWindowRect(m_windowHandler, &m_windowRectangle);
		if (result)
		{
			LONG newWidth = m_windowRectangle.right - m_windowRectangle.left;
			LONG newHeight = m_windowRectangle.bottom - m_windowRectangle.top;

			if (m_height == newHeight && m_width == newWidth)
				return false;
			m_height = newHeight;
			m_width = newWidth;

			ThomasCore::GetDeviceContext()->OMSetRenderTargets(0, 0, 0);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(NULL, 1);
			SAFE_RELEASE(m_dxBuffers.backBuffer);
			SAFE_RELEASE(m_dxBuffers.backBufferSRV);
			SAFE_RELEASE(m_dxBuffers.depthStencilState);
			SAFE_RELEASE(m_dxBuffers.depthStencilView);
			SAFE_RELEASE(m_dxBuffers.depthStencilViewReadOnly);
			SAFE_RELEASE(m_dxBuffers.depthBufferSRV);

			m_swapChain->ResizeBuffers(0, m_width, m_height, DXGI_FORMAT_UNKNOWN, 0);
			if (s_current == this)
				s_current = nullptr;
			return InitDxBuffers();
		}
		else
			return false;
	}


	bool Window::UpdateWindow()
	{
		return false;
	}

	bool Window::Initialized()
	{
		return m_initialized;
	}

	void Window::Destroy()
	{
		if (s_editorWindow)
			delete s_editorWindow;

		for (Window* window : s_windows)
			delete window;

		s_windows.clear();
	}

	void Window::ClearAllWindows()
	{
		if (s_editorWindow && s_editorWindow->Initialized())
			s_editorWindow->Clear();

		for (Window* window : s_windows)
			if (window->Initialized())
				window->Clear();
	}

	void Window::PresentAllWindows()
	{
		if (s_editorWindow && s_editorWindow->Initialized())
		{
			s_editorWindow->Bind();
			if (ImGui_ImplDx11_Valid() && s_editorWindow->m_guiData)
				ImGui_ImplDX11_RenderDrawData(s_editorWindow->m_guiData);
			s_editorWindow->Present();
		}

		for (Window* window : s_windows)
			if (window->Initialized())
				window->Present();

		s_current = nullptr;
	}

	void Window::QueueResize()
	{
		m_shouldResize = true;
	}

	bool Window::WaitingForUpdate()
	{
		if (s_editorWindow && s_editorWindow->m_shouldResize)
			return true;

		for (Window* window : s_windows)
			if (window->m_shouldResize)
				return true;

		return false;
	}

	void Window::Update()
	{
		if (s_editorWindow)
		{
			if (!s_editorWindow->m_initialized)
			{
				s_editorWindow->InitDxBuffers();
				s_editorWindow->m_initialized = true;
			}
			if (s_editorWindow->m_shouldResize)
			{
				ImGui_ImplDX11_InvalidateDeviceObjects();
				s_editorWindow->m_initialized = false;
				s_editorWindow->Resize();
				s_editorWindow->m_initialized = true; //Hmm...?
				s_editorWindow->m_shouldResize = false;
				ImGui_ImplDX11_CreateDeviceObjects();
			}
			BeginFrame();
		}

		for (Window* window : s_windows)
		{
			if (!window->m_initialized)
			{
				window->InitDxBuffers();
				window->m_initialized = true;
			}
			if (window->m_shouldResize)
			{
				window->Resize();
				window->m_shouldResize = false;
			}
		}
	}

	void Window::UpdateFocus()
	{
		if (s_editorWindow)
			s_editorWindow->m_focused = s_editorWindow->GetWindowHandler() == GetFocus();

		for (Window* window : s_windows)
			window->m_focused = window->GetWindowHandler() == GetFocus();
	}

	void Window::Bind()
	{
		if (s_current != this)
		{
			ThomasCore::GetDeviceContext()->OMSetRenderTargets(1, &m_dxBuffers.backBuffer, m_dxBuffers.depthStencilView);
			ThomasCore::GetDeviceContext()->OMSetDepthStencilState(m_dxBuffers.depthStencilState, 1);
			s_current = this;
		}
	}

	bool Window::ChangeWindowShowState(int nCmdShow)
	{
		return ShowWindow(m_windowHandler, nCmdShow);
	}

	void Window::Present()
	{
		m_swapChain->Present(0, 0);
	}

	bool Window::InitDxBuffers()
	{
		bool hr = utils::D3d::CreateBackBuffer(ThomasCore::GetDevice(), m_swapChain, m_dxBuffers.backBuffer, m_dxBuffers.backBufferSRV);
		if (hr)
		{
			hr = utils::D3d::CreateDepthStencilView(ThomasCore::GetDevice(), m_width, m_height, m_dxBuffers.depthStencilView, m_dxBuffers.depthStencilViewReadOnly, m_dxBuffers.depthBufferSRV);
			if (hr)
			{
				hr = m_dxBuffers.depthStencilState = utils::D3d::CreateDepthStencilState(D3D11_COMPARISON_LESS, true);
				if (hr)
					return true;
			}
		}
		LOG("Failed to create DirectX window buffers");
		return false;	
	}

	bool Window::IsFocused()
	{
		return m_focused;
	}

	void Window::Clear()
	{
		float clearColor[] = { 0.34375f, 0.34375f, 0.34375f, 1.0f };
		ThomasCore::GetDeviceContext()->ClearRenderTargetView(m_dxBuffers.backBuffer, clearColor);
		ThomasCore::GetDeviceContext()->ClearDepthStencilView(m_dxBuffers.depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	}

	void Window::SetCursor(const bool & visible)
	{
		if (m_showCursor != visible)
		{
			m_showCursor = visible;
			ShowCursor(visible);
		}
	}

	bool Window::SetHeight(const LONG & height)
	{
		if (height > 0 && height <= GetVerticalResolution())
		{
			m_height = height;
			return UpdateWindow();
		}

		return false;
	}

	bool Window::SetWidth(const LONG & width)
	{
		if (width > 0 && width <= GetHorizontalResolution())
		{
			m_width = width;
			return UpdateWindow();
		}

		return false;
	}

	bool Window::SetSize(const LONG & height, const LONG & width)
	{
		if (SetWidth(width) && SetHeight(height))
			return true;

		return false;
	}

	Window * Window::GetEditorWindow()
	{
		return s_editorWindow;
	}

	Window * Window::GetWindow(int index)
	{
		if (index == -1 && s_editorWindow)
			return s_editorWindow;
		else if (index < s_windows.size())
			return s_windows[index];

		return nullptr;
	}

	Window * Window::GetWindow(HWND hWnd)
	{
		//Maybe define editor or something?
		if (s_editorWindow && s_editorWindow->GetWindowHandler() == hWnd)
			return s_editorWindow;

		for (auto window : s_windows)
			if (window->GetWindowHandler() == hWnd)
				return window;

		return nullptr;
	}

	std::vector<Window*> Window::GetWindows()
	{
		return s_windows;
	}


	IDXGISwapChain * Window::GetSwapChain() const
	{
		return m_swapChain;
	}

	LONG Window::GetHeight() const
	{
		return m_height;
	}

	LONG Window::GetWidth() const
	{
		return m_width;
	}

	float Window::GetRealAspectRatio() const
	{
		return (float)m_width / (float)m_height;
	}

	HWND Window::GetWindowHandler() const
	{
		return m_windowHandler;
	}

	LONG Window::GetHorizontalResolution() const
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		return desktop.right;
	}

	LONG Window::GetVerticalResolution() const
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		return desktop.bottom;
	}

	Window * Window::GetCurrentBound()
	{
		return s_current;
	}	

	//Imgui related functionality for Gizmos
	void Window::CloneGUIData()
	{
		ImDrawData* data = ImGui::GetDrawData();
		ImDrawData* dst = new ImDrawData();
		dst->Valid = data->Valid;
		dst->CmdListsCount = data->CmdListsCount;
		dst->TotalIdxCount = data->TotalIdxCount;
		dst->TotalVtxCount = data->TotalVtxCount;

		dst->CmdLists = (ImDrawList**)malloc(data->CmdListsCount * sizeof(ImDrawList*));
		for (int i = 0; i < data->CmdListsCount; ++i)
			dst->CmdLists[i] = data->CmdLists[i]->CloneOutput();

		m_guiData = dst;
	}

	void Window::DeleteGUIData()
	{
		if (m_guiData)
		{
			for (int i = 0; i < m_guiData->CmdListsCount; ++i)
				delete m_guiData->CmdLists[i];

			m_guiData->Clear();
			delete m_guiData;
			m_guiData = nullptr;
		}
	}

	void Window::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGuizmo::BeginFrame();
	}

	void Window::EndFrame(bool copyGui)
	{
		if (s_editorWindow)
		{
			if (copyGui)
			{
				ImGui::Render();
				s_editorWindow->DeleteGUIData();
				s_editorWindow->CloneGUIData();
			}
			else
				ImGui::EndFrame();	
		}
	}

	//Windows window events function
	LRESULT CALLBACK Window::EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		Window* window = GetWindow(hWnd);

		//If one case is hit the code will execute everything down until a break;
		switch (message)
		{
		case WM_SIZE:
		{
			if (window)
				window->QueueResize();
		}
		break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			Input::ProcessGamePad(message, wParam, lParam);
			break;
		case WM_ACTIVATEAPP:
			Input::ProcessKeyboard(message, wParam, lParam);
			Input::ProcessMouse(message, wParam, lParam, hWnd);
			break;
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
			SetFocus(hWnd);
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Input::ProcessMouse(message, wParam, lParam, hWnd);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Input::ProcessKeyboard(message, wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
