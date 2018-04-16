#pragma once
#pragma unmanaged
#include <thomas\Input.h>
#pragma managed
#include <string>
#include <msclr\marshal_cppstd.h>
#include "math\Math.h"


namespace ThomasEditor
{
	public ref class Input
	{
	public:
		enum Keyboard::Keys;

		using MouseButtons = thomas::Input::MouseButtons;
		using Buttons = thomas::Input::Buttons;

	public:
		static void Vibrate(float left, float right) { return thomas::Input::Vibrate(left, right); }
		static void Vibrate(float left, float right, float time) { return thomas::Input::Vibrate(left, right, time); }

	public:
		static void SetMouseMode(Mouse::Mode mode) { return thomas::Input::SetMouseMode(mode); }

	public:
		//Mouse
		static int GetMouseScrollWheel() { return thomas::Input::GetMouseScrollWheel(); };
		static bool GetMouseButtonDown(MouseButtons button) { return thomas::Input::GetMouseButtonDown(button); }
		static bool GetMouseButtonUp(MouseButtons button) { return thomas::Input::GetMouseButtonUp(button); }
		static bool GetMouseButton(MouseButtons button) { return thomas::Input::GetMouseButton(button); }
		static Vector2 GetMousePosition() { return Vector2(thomas::Input::GetMousePosition()); }

		//Keyboard
		static bool GetKeyDown(Keyboard::Keys key) { return thomas::Input::GetKeyDown(key); }
		static bool GetKeyUp(Keyboard::Keys key) { return thomas::Input::GetKeyUp(key); }
		static bool GetKey(Keyboard::Keys key) { return thomas::Input::GetKey(key); }

		//Gamepad
		static bool GetButtonDown(Buttons button) { return thomas::Input::GetButtonDown(button); }
		static bool GetButtonUp(Buttons button) { return thomas::Input::GetButtonUp(button); }
		static bool GetButton(Buttons button) {	return thomas::Input::GetButton(button); }
		static float GetLeftStickY() { return thomas::Input::GetLeftStickY(); }
		static float GetLeftStickX() { return thomas::Input::GetLeftStickX(); }
		static float GetRightStickY() { return thomas::Input::GetRightStickY(); }
		static float GetRightStickX() { return thomas::Input::GetRightStickX(); }
		static float GetLeftTriggerDelta() { return thomas::Input::GetLeftTriggerDelta(); }
		static float GetRightTriggerDelta() { return thomas::Input::GetRightTriggerDelta(); }	
	};
}