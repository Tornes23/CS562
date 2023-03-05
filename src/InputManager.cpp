#include "InputManager.h"
#include "Camera.h"
#include "Window.h"

#pragma region // General Input Set Up //
void InputHandler::Initialize()
{
	for (unsigned i = 0; i < MOUSE_KEY_AMOUNT; ++i)
	{
		mMouseCurrent[i] = 0;
		mMousePrevious[i] = 0;
	}

	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i)
	{
		mKeyCurrent[i] = 0;
		mKeyPrevious[i] = 0;
	}
}
void InputHandler::StartFrame()
{
	/* Reset the Current and Previous arrays */
	for (unsigned i = 0; i < MOUSE_KEY_AMOUNT; ++i)
		mMousePrevious[i] = mMouseCurrent[i];

	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i)
		mKeyPrevious[i] = mKeyCurrent[i];

	mWheelCurrent = false;
	mWheelScroll = 0;
}

#pragma endregion

void InputHandler::HandleEvents()
{
	GetRawMouse();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
#ifdef EDITOR
		ImGui_ImplSDL2_ProcessEvent(&event);
#endif

		switch (event.type)
		{
		case SDL_QUIT:
			Window.CloseWindow();
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputManager.HandleKeyEvent(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			InputManager.HandleMouseEvent(event);
			break;
		case SDL_MOUSEWHEEL:
			InputManager.HandleMouseWheel(event);
			break;
		case SDL_MOUSEMOTION:
			mRelMouse = glm::ivec2(event.motion.xrel, event.motion.yrel);
			break;
		}
	}
}

#pragma region // MOUSE //

void InputHandler::HandleMouseEvent(SDL_Event event)
{
	// Access the index with -1 beacuse they go:
	// LEFT = 1, MIDDLE = 2, RIGHT = 3
	
	mMouseCurrent[event.button.button - 1] = event.button.state ? true : false;
}
bool InputHandler::WheelTriggered()
{
	return mWheelCurrent;
}
bool InputHandler::MouseIsDown(MouseKey index)
{
	return mMouseCurrent[static_cast<unsigned>(index)];
}
bool InputHandler::MouseIsUp(MouseKey index)
{
	return !mMouseCurrent[static_cast<unsigned>(index)];
}
bool InputHandler::MouseIsTriggered(MouseKey index)
{
	if (mMouseCurrent[static_cast<unsigned>(index)] == true)
	{
		if (mMouseCurrent[static_cast<unsigned>(index)] != mMousePrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}
bool InputHandler::MouseIsReleased(MouseKey index)
{
	if (mMouseCurrent[static_cast<unsigned>(index)] == false)
	{
		if (mMouseCurrent[static_cast<unsigned>(index)] != mMousePrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}

int InputHandler::GetWheelScroll() const
{
	return mWheelScroll;
}

void InputHandler::HandleMouseWheel(SDL_Event event)
{
	mWheelCurrent = true;
	mWheelScroll = event.wheel.y;
}

#pragma endregion

#pragma region // KEYBOARD //
#include "imgui/imgui.h"

void InputHandler::HandleKeyEvent(SDL_Event event)
{
	SDL_Keycode ScanCode = event.key.keysym.scancode;

	if (ScanCode > 0 && ScanCode < KEYBOARD_KEY_AMOUNT)
		mKeyCurrent[ScanCode] = event.key.state ? true : false;
}

bool InputHandler::KeyIsDown(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	return mKeyCurrent[static_cast<unsigned>(index)];
}
bool InputHandler::KeyIsUp(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;
	return !KeyIsDown(index);
}
bool InputHandler::KeyIsTriggered(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	if (mKeyCurrent[static_cast<unsigned>(index)] == true)
	{
		if (mKeyCurrent[static_cast<unsigned>(index)] != mKeyPrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}
bool InputHandler::KeyIsReleased(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	if (mKeyCurrent[static_cast<unsigned>(index)] == false)
	{
		if (mKeyCurrent[static_cast<unsigned>(index)] != mKeyPrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}
#pragma endregion

#pragma region // MOUSE POSITIONS //

const glm::ivec2& InputHandler::RawMousePos() const
{
	return mRawMouse;
}
const glm::vec2& InputHandler::WindowMousePos() const
{
	return mWindowMouse;
}

void InputHandler::SetMousePos(glm::vec2 pos)
{
	mWindowMouse = pos;
}

void InputHandler::GetRawMouse()
{
	SDL_GetMouseState(&mRawMouse[0], &mRawMouse[1]);
	mWindowMouse = mRawMouse - Window.GetViewport() / 2;
	mWindowMouse.y *= -1;
}

#pragma endregion


