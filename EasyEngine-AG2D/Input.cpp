#include "EasyEngineAG2D.h"

namespace EE {
	Input::Input(HWND window)
	{
		// save the window Handle
		this->window = window;
		// create direct Input object
		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL);

		// Initialize keyboard
		di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
		keyboard->SetDataFormat(&c_dfDIKeyboard);
		keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		keyboard->Acquire();

		// Initialize mouse
		di->CreateDevice(GUID_SysMouse, &mouse, NULL);
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		mouse->Acquire();

	}
	Input::~Input()
	{
		di->Release();
		keyboard->Release();
		mouse->Release();
	}
	void Input::Update()
	{
		// Poll state of the keyboard
		keyboard->Poll();
		if (!SUCCEEDED(keyboard->GetDeviceState(256, (LPVOID)keyState)))
		{
			// Keyboard device lost . try to re-acquire it
			keyboard->Acquire();
		}

		// Poll state of the mouse
		mouse->Poll();
		if (!SUCCEEDED(mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
		{
			// Mouse device lost. try to re-acquire it
			mouse->Acquire();
		}

		// Get mouse position on screen
		GetCursorPos(&position);
		ScreenToClient(window, &position);

	}
	bool Input::GetMouseButton(char button)
	{
		return (mouseState.rgbButtons[button] & 0x80);
	}
}; //  namespace