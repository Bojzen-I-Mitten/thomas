/*
	Input manager class wrapper around DirectXTK
*/

#pragma once
#include <Windows.h>
#include <DirectXTK\GamePad.h>
#include <DirectXTK\Keyboard.h>
#include <DirectXTK\Mouse.h>
#include "utils\Math.h"

using namespace DirectX;

namespace thomas
{
	class Input
	{
	public:
		enum class MouseButtons
		{
			LEFT,
			RIGHT,
			MIDDLE
		};

		enum class Buttons
		{
			A,
			B,
			X,
			Y,
			DPAD_UP,
			DPAD_DOWN,
			DPAD_LEFT,
			DPAD_RIGHT,
			LS,
			RS,
			LB,
			RB,
			LT,
			RT,
			START,
			BACK
		};

	public:	
		static bool Init();
		static void Update();
		static void ProcessKeyboard(UINT message, WPARAM wParam, LPARAM lParam);
		static void ProcessMouse(UINT message, WPARAM wParam, LPARAM lParam, HWND handle);
		static void ProcessGamePad(UINT message, WPARAM wParam, LPARAM lParam);
		static void ResetScrollWheelValue();
		static void Vibrate(float left, float right, float time = 0.f);

	public:
		static void SetMouseMode(Mouse::Mode mode);

	public:
		//Mouse
		static bool GetMouseButtonDown(MouseButtons button);
		static bool GetMouseButtonUp(MouseButtons button);
		static bool GetMouseButton(MouseButtons button);
		static math::Vector2 GetMousePosition();
		static int GetMouseScrollWheel();

		//Keyboard
		static bool GetKeyDown(Keyboard::Keys key);
		static bool GetKeyUp(Keyboard::Keys key);
		static bool GetKey(Keyboard::Keys key);
	
		//Gamepad
		static bool GetButtonDown(Buttons button);
		static bool GetButtonUp(Buttons button);
		static bool GetButton(Buttons button);
		static float GetLeftStickY();
		static float GetLeftStickX();
		static float GetRightStickY();
		static float GetRightStickX();
		static float GetLeftTriggerDelta();
		static float GetRightTriggerDelta();

	private:
		//Keyboard
		static std::unique_ptr<Keyboard> s_keyboard;
		static Keyboard::State s_keyboardState;
		static Keyboard::KeyboardStateTracker s_keyboardTracker;

	private:
		//Mouse
		static std::unique_ptr<Mouse> s_mouse;
		static Mouse::State s_mouseState;
		static Mouse::ButtonStateTracker s_mouseTracker;

	private:
		//Gamepad
		static std::unique_ptr<GamePad> s_gamePad;	
		static GamePad::State s_gamePadState;	
		static GamePad::ButtonStateTracker s_gamePadTracker;

	private:
		static bool s_initialized;
		static bool s_recordPosition;
		static Mouse::Mode s_mouseMode;
		static math::Vector2 s_mousePosition;
		static float s_vibrateTimeLeft;
	};
}