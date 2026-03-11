#include "Controller.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>

static unsigned int ToXInputButton(dae::Controller::Button button)
{
	switch (button)
	{
	case dae::Controller::Button::DPadUp:        return XINPUT_GAMEPAD_DPAD_UP;
	case dae::Controller::Button::DPadDown:      return XINPUT_GAMEPAD_DPAD_DOWN;
	case dae::Controller::Button::DPadLeft:      return XINPUT_GAMEPAD_DPAD_LEFT;
	case dae::Controller::Button::DPadRight:     return XINPUT_GAMEPAD_DPAD_RIGHT;
	case dae::Controller::Button::Start:         return XINPUT_GAMEPAD_START;
	case dae::Controller::Button::Back:          return XINPUT_GAMEPAD_BACK;
	case dae::Controller::Button::LeftThumb:     return XINPUT_GAMEPAD_LEFT_THUMB;
	case dae::Controller::Button::RightThumb:    return XINPUT_GAMEPAD_RIGHT_THUMB;
	case dae::Controller::Button::LeftShoulder:  return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case dae::Controller::Button::RightShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case dae::Controller::Button::ButtonA:       return XINPUT_GAMEPAD_A;
	case dae::Controller::Button::ButtonB:       return XINPUT_GAMEPAD_B;
	case dae::Controller::Button::ButtonX:       return XINPUT_GAMEPAD_X;
	case dae::Controller::Button::ButtonY:       return XINPUT_GAMEPAD_Y;
	default:                                     return 0;
	}
}

class dae::Controller::Impl
{
public:
	explicit Impl(unsigned int playerIndex)
		: m_playerIndex(playerIndex)
	{
	}

	void Update()
	{
		m_previousState = m_currentState;
		ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_playerIndex, &m_currentState);

		const auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
		m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
		m_buttonsReleasedThisFrame = buttonChanges & ~m_currentState.Gamepad.wButtons;
	}

	bool IsDown(Button button) const
	{
		return m_currentState.Gamepad.wButtons & ToXInputButton(button);
	}

	bool IsUp(Button button) const
	{
		return m_buttonsReleasedThisFrame & ToXInputButton(button);
	}

	bool IsPressed(Button button) const
	{
		return m_buttonsPressedThisFrame & ToXInputButton(button);
	}

private:
	unsigned int m_playerIndex;
	XINPUT_STATE m_currentState{};
	XINPUT_STATE m_previousState{};
	unsigned int m_buttonsPressedThisFrame{};
	unsigned int m_buttonsReleasedThisFrame{};
};

#else

#include <SDL3/SDL.h>

// SDL gamepad implementation for Emscripten
class dae::Controller::Impl
{
public:
	explicit Impl(unsigned int playerIndex)
		: m_playerIndex(playerIndex)
	{
		TryOpenGamepad();
	}

	~Impl()
	{
		if (m_gamepad)
			SDL_CloseGamepad(m_gamepad);
	}

	void Update()
	{
		SDL_UpdateGamepads();

		// Gamepad may connect after startup
		if (!m_gamepad)
			TryOpenGamepad();

		m_previousButtons = m_currentButtons;
		m_currentButtons = 0;

		if (m_gamepad)
		{
			auto setIf = [&](Controller::Button btn, SDL_GamepadButton sdlBtn)
				{
					if (SDL_GetGamepadButton(m_gamepad, sdlBtn))
						m_currentButtons |= static_cast<unsigned int>(btn);
				};

			setIf(Controller::Button::DPadUp, SDL_GAMEPAD_BUTTON_DPAD_UP);
			setIf(Controller::Button::DPadDown, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
			setIf(Controller::Button::DPadLeft, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
			setIf(Controller::Button::DPadRight, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
			setIf(Controller::Button::Start, SDL_GAMEPAD_BUTTON_START);
			setIf(Controller::Button::Back, SDL_GAMEPAD_BUTTON_BACK);
			setIf(Controller::Button::LeftThumb, SDL_GAMEPAD_BUTTON_LEFT_STICK);
			setIf(Controller::Button::RightThumb, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
			setIf(Controller::Button::LeftShoulder, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
			setIf(Controller::Button::RightShoulder, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
			setIf(Controller::Button::ButtonA, SDL_GAMEPAD_BUTTON_SOUTH);
			setIf(Controller::Button::ButtonB, SDL_GAMEPAD_BUTTON_EAST);
			setIf(Controller::Button::ButtonX, SDL_GAMEPAD_BUTTON_WEST);
			setIf(Controller::Button::ButtonY, SDL_GAMEPAD_BUTTON_NORTH);
		}

		const unsigned int buttonChanges = m_currentButtons ^ m_previousButtons;
		m_buttonsPressedThisFrame = buttonChanges & m_currentButtons;
		m_buttonsReleasedThisFrame = buttonChanges & ~m_currentButtons;
	}

	bool IsDown(Button button) const
	{
		return m_currentButtons & static_cast<unsigned int>(button);
	}

	bool IsUp(Button button) const
	{
		return m_buttonsReleasedThisFrame & static_cast<unsigned int>(button);
	}

	bool IsPressed(Button button) const
	{
		return m_buttonsPressedThisFrame & static_cast<unsigned int>(button);
	}

private:
	void TryOpenGamepad()
	{
		int count = 0;
		SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
		if (gamepads && m_playerIndex < static_cast<unsigned int>(count))
			m_gamepad = SDL_OpenGamepad(gamepads[m_playerIndex]);
		SDL_free(gamepads);
	}

	unsigned int  m_playerIndex;
	SDL_Gamepad* m_gamepad{ nullptr };
	unsigned int  m_currentButtons{};
	unsigned int  m_previousButtons{};
	unsigned int  m_buttonsPressedThisFrame{};
	unsigned int  m_buttonsReleasedThisFrame{};
};

#endif

dae::Controller::Controller(unsigned int playerIndex)
	: m_pImpl(std::make_unique<Impl>(playerIndex))
{
}


dae::Controller::~Controller() = default;

void dae::Controller::Update() { m_pImpl->Update(); }
bool dae::Controller::IsDown(Button b) const { return m_pImpl->IsDown(b); }
bool dae::Controller::IsUp(Button b) const { return m_pImpl->IsUp(b); }
bool dae::Controller::IsPressed(Button b) const { return m_pImpl->IsPressed(b); }