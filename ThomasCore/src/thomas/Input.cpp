#include "Input.h"
#include "ThomasTime.h"
#include "Common.h"

namespace thomas
{
	//Keyboard
	std::unique_ptr<Keyboard> Input::s_keyboard;
	Keyboard::KeyboardStateTracker Input::s_keyboardTracker;
	Keyboard::State Input::s_keyboardState;

	//Mouse
	std::unique_ptr<Mouse> Input::s_mouse;
	Mouse::State Input::s_mouseState;
	Mouse::ButtonStateTracker Input::s_mouseTracker;

	//Gamepad
	std::unique_ptr<GamePad> Input::s_gamePad;	
	GamePad::State Input::s_gamePadState;
	GamePad::ButtonStateTracker Input::s_gamePadTracker;

	math::Vector2 Input::s_mousePosition;
	Input::MouseMode Input::s_mouseMode;
	bool Input::s_recordPosition = true;
	bool Input::s_initialized = false;
	float Input::s_vibrateTimeLeft = 0.f;

	bool Input::Init()
	{
		//Init objects
		s_keyboard = std::make_unique<Keyboard>();
		s_mouse = std::make_unique<Mouse>();
		s_gamePad = std::make_unique<GamePad>();
			
		s_mouseMode = MouseMode::POSITION_ABSOLUTE;
		s_initialized = true;
		LOG("Initiating Input");
		return s_initialized;
	}

	void Input::Update()
	{
		if (s_initialized)
		{
			s_keyboardState = s_keyboard->GetState();
			s_gamePadState = s_gamePad->GetState(0);	
			s_mouseState = s_mouse->GetState();
			s_vibrateTimeLeft -= ThomasTime::GetDeltaTime();

			if(s_vibrateTimeLeft < 0.f)
				s_gamePad->SetVibration(0, 0.f, 0.f);

			s_keyboardTracker.Update(s_keyboardState);
			s_mouseTracker.Update(s_mouseState);
			s_gamePadTracker.Update(s_gamePadState);
			s_mousePosition = math::Vector2((float)s_mouseState.x, (float)s_mouseState.y);
						
			if (s_mousePosition == math::Vector2(0.f, 0.f))
				s_recordPosition = true;
		}
	}

	void Input::ProcessKeyboard(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (s_initialized)
			s_keyboard->ProcessMessage(message, wParam, lParam);
	}

	void Input::ProcessMouse(UINT message, WPARAM wParam, LPARAM lParam, HWND handle)
	{
		if (s_initialized)
		{
			s_mouse->SetWindow(handle);
			s_mouse->ProcessMessage(message, wParam, lParam);
		}			
	}

	void Input::ProcessGamePad(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (s_initialized)
		{
			//Needs fixing
	/*		switch (message)
			{

			case WM_SETFOCUS:
			{
				s_gamePad->Resume();
			}
				
			case WM_KILLFOCUS:
				s_gamePad->Suspend();
				break;
			}*/
		}
	}

	void Input::ResetScrollWheelValue()
	{
		s_mouse->ResetScrollWheelValue();
	}

	//Gamepad
	float Input::GetLeftStickY()
	{
		return s_gamePadState.thumbSticks.leftY;
	}

	float Input::GetLeftStickX()
	{
		return s_gamePadState.thumbSticks.leftX;
	}

	float Input::GetRightStickY()
	{
		return s_gamePadState.thumbSticks.rightY;
	}

	float Input::GetRightStickX()
	{
		return s_gamePadState.thumbSticks.rightX;
	}

	float Input::GetLeftTriggerDelta()
	{
		return s_gamePadState.triggers.left;
	}

	float Input::GetRightTriggerDelta()
	{
		return s_gamePadState.triggers.right;
	}

	//Mouse
	void Input::SetMouseMode(MouseMode mode)
	{
		if (s_mouseMode == mode)
			return;

		s_mouse->SetMode((Mouse::Mode)mode); s_mouseMode = mode;
		mode == MouseMode::POSITION_RELATIVE ? s_recordPosition = false : s_recordPosition = true;
	}

	math::Vector2 Input::GetMousePosition()
	{
		if (s_recordPosition)
			return s_mousePosition;
		else
			return math::Vector2(0.f, 0.f);
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	bool Input::GetMouseButtonDown(MouseButtons button)
	{
		switch (button)
		{
		case MouseButtons::LEFT:
			return (s_mouseTracker.leftButton == s_mouseTracker.PRESSED);
		case MouseButtons::RIGHT:
			return (s_mouseTracker.rightButton == s_mouseTracker.PRESSED);
		case MouseButtons::MIDDLE:
			return (s_mouseTracker.middleButton == s_mouseTracker.PRESSED);
		}
		return false;
	}

	bool Input::GetMouseButtonUp(MouseButtons button)
	{
		switch (button)
		{
		case MouseButtons::LEFT:
			return (s_mouseTracker.leftButton == s_mouseTracker.RELEASED);
		case MouseButtons::RIGHT:
			return (s_mouseTracker.rightButton == s_mouseTracker.RELEASED);
		case MouseButtons::MIDDLE:
			return (s_mouseTracker.middleButton == s_mouseTracker.RELEASED);
		}
		return false;
	}

	bool Input::GetMouseButton(MouseButtons button)
	{
		switch (button)
		{
		case MouseButtons::LEFT:
			return s_mouseState.leftButton;
		case MouseButtons::RIGHT:
			return s_mouseState.rightButton;
		case MouseButtons::MIDDLE:
			return s_mouseState.middleButton;
		}
		return false;
	}

	int Input::GetMouseScrollWheel()
	{
		return s_mouseState.scrollWheelValue;
	}

	bool Input::GetButtonDown(Buttons button)
	{
		if (!s_gamePadState.IsConnected()) //Always false if no gamePad.
			return false;

		switch (button)
		{
		case Buttons::A:
			return (s_gamePadTracker.a == s_gamePadTracker.PRESSED);
		case Buttons::B:
			return (s_gamePadTracker.b == s_gamePadTracker.PRESSED);
		case Buttons::X:
			return (s_gamePadTracker.x == s_gamePadTracker.PRESSED);
		case Buttons::Y:
			return (s_gamePadTracker.y == s_gamePadTracker.PRESSED);
		case Buttons::DPAD_UP:
			return (s_gamePadTracker.dpadUp == s_gamePadTracker.PRESSED);
		case Buttons::DPAD_DOWN:
			return (s_gamePadTracker.dpadDown == s_gamePadTracker.PRESSED);
		case Buttons::DPAD_LEFT:
			return (s_gamePadTracker.dpadLeft == s_gamePadTracker.PRESSED);
		case Buttons::DPAD_RIGHT:
			return (s_gamePadTracker.dpadRight == s_gamePadTracker.PRESSED);
		case Buttons::LS:
			return (s_gamePadTracker.leftStick == s_gamePadTracker.PRESSED);
		case Buttons::RS:
			return (s_gamePadTracker.rightStick == s_gamePadTracker.PRESSED);
		case Buttons::LB:
			return (s_gamePadTracker.leftShoulder == s_gamePadTracker.PRESSED);
		case Buttons::RB:
			return (s_gamePadTracker.rightShoulder == s_gamePadTracker.PRESSED);
		case Buttons::LT:
			return (s_gamePadTracker.leftTrigger == s_gamePadTracker.PRESSED);
		case Buttons::RT:
			return (s_gamePadTracker.rightTrigger == s_gamePadTracker.PRESSED);
		case Buttons::START:
			return (s_gamePadTracker.start == s_gamePadTracker.PRESSED);
		case Buttons::BACK:
			return (s_gamePadTracker.back == s_gamePadTracker.PRESSED);
		}
		return false;
	}

	bool Input::GetButtonUp(Buttons button)
	{
		if (!s_gamePadState.IsConnected()) //Always false if no gamePad.
			return false;

		switch (button)
		{
		case Buttons::A:
			return (s_gamePadTracker.a == s_gamePadTracker.RELEASED);
		case Buttons::B:
			return (s_gamePadTracker.b == s_gamePadTracker.RELEASED);
		case Buttons::X:
			return (s_gamePadTracker.x == s_gamePadTracker.RELEASED);
		case Buttons::Y:
			return (s_gamePadTracker.y == s_gamePadTracker.RELEASED);
		case Buttons::DPAD_UP:
			return (s_gamePadTracker.dpadUp == s_gamePadTracker.RELEASED);
		case Buttons::DPAD_DOWN:
			return (s_gamePadTracker.dpadDown == s_gamePadTracker.RELEASED);
		case Buttons::DPAD_LEFT:
			return (s_gamePadTracker.dpadLeft == s_gamePadTracker.RELEASED);
		case Buttons::DPAD_RIGHT:
			return (s_gamePadTracker.dpadRight == s_gamePadTracker.RELEASED);
		case Buttons::LS:
			return (s_gamePadTracker.leftStick == s_gamePadTracker.RELEASED);
		case Buttons::RS:
			return (s_gamePadTracker.rightStick == s_gamePadTracker.RELEASED);
		case Buttons::LB:
			return (s_gamePadTracker.leftShoulder == s_gamePadTracker.RELEASED);
		case Buttons::RB:
			return (s_gamePadTracker.rightShoulder == s_gamePadTracker.RELEASED);
		case Buttons::LT:
			return (s_gamePadTracker.leftTrigger == s_gamePadTracker.RELEASED);
		case Buttons::RT:
			return (s_gamePadTracker.rightTrigger == s_gamePadTracker.RELEASED);
		case Buttons::START:
			return (s_gamePadTracker.start == s_gamePadTracker.RELEASED);
		case Buttons::BACK:
			return (s_gamePadTracker.back == s_gamePadTracker.RELEASED);
		}
		return false;
	}

	bool Input::GetButton(Buttons button)
	{
		if (!s_gamePadState.IsConnected()) //Always false if no gamePad.
			return false;

		switch (button)
		{
		case Buttons::A:
			//return s_gamePadState.IsAPressed();
			return (s_gamePadTracker.a == s_gamePadTracker.HELD);
		case Buttons::B:
			return (s_gamePadTracker.b == s_gamePadTracker.HELD);
		case Buttons::X:
			return (s_gamePadTracker.x == s_gamePadTracker.HELD);
		case Buttons::Y:
			return (s_gamePadTracker.y == s_gamePadTracker.HELD);
		case Buttons::DPAD_UP:
			return (s_gamePadTracker.dpadUp == s_gamePadTracker.HELD);
		case Buttons::DPAD_DOWN:
			return (s_gamePadTracker.dpadDown == s_gamePadTracker.HELD);
		case Buttons::DPAD_LEFT:
			return (s_gamePadTracker.dpadLeft == s_gamePadTracker.HELD);
		case Buttons::DPAD_RIGHT:
			return (s_gamePadTracker.dpadRight == s_gamePadTracker.HELD);
		case Buttons::LS:
			return (s_gamePadTracker.leftStick == s_gamePadTracker.HELD);
		case Buttons::RS:
			return (s_gamePadTracker.rightStick == s_gamePadTracker.HELD);
		case Buttons::LB:
			return (s_gamePadTracker.leftShoulder == s_gamePadTracker.HELD);
		case Buttons::RB:
			return (s_gamePadTracker.rightShoulder == s_gamePadTracker.HELD);
		case Buttons::LT:
			return (s_gamePadTracker.leftTrigger == s_gamePadTracker.HELD);
		case Buttons::RT:
			return (s_gamePadTracker.rightTrigger == s_gamePadTracker.HELD);
		case Buttons::START:
			return (s_gamePadTracker.start == s_gamePadTracker.HELD);
		case Buttons::BACK:
			return (s_gamePadTracker.back == s_gamePadTracker.HELD);
		}
		return false;
	}

	void Input::Vibrate(float left, float right, float time)
	{
		if (s_gamePadState.IsConnected()) //Always false if no gamePad.
		{
			s_gamePad->SetVibration(0, left, right);
			s_vibrateTimeLeft = time;
		}		
	}

	//Keyboard
	bool Input::GetKeyDown(Keys key)
	{
		return s_keyboardTracker.IsKeyPressed((Keyboard::Keys)key);
	}	

	bool Input::GetKeyUp(Keys key)
	{
		return s_keyboardTracker.IsKeyReleased((Keyboard::Keys)key);
	}

	bool Input::GetKey(Keys key)
	{
		return s_keyboardState.IsKeyDown((Keyboard::Keys)key);
	}
}
