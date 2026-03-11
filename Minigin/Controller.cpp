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

	void ProcessEvent(const SDL_Event&) {} 

private:
	unsigned int m_playerIndex;
	XINPUT_STATE m_currentState{};
	XINPUT_STATE m_previousState{};
	unsigned int m_buttonsPressedThisFrame{};
	unsigned int m_buttonsReleasedThisFrame{};
};

#else

#include <SDL3/SDL.h>


class dae::Controller::Impl
{
public:
	explicit Impl(unsigned int playerIndex)
		: m_playerIndex(playerIndex)
	{
	}


	void Update()
	{
		m_previousButtons = m_currentButtons;
		m_buttonsPressedThisFrame = 0;
		m_buttonsReleasedThisFrame = 0;
	}


	void ProcessEvent(const SDL_Event& e)
	{
		if (e.type == SDL_EVENT_GAMEPAD_ADDED)
		{
		
			if (!m_gamepad)
			{
			
				int count = 0;
				SDL_JoystickID* ids = SDL_GetGamepads(&count);
				if (ids && m_playerIndex < static_cast<unsigned int>(count))
					m_gamepad = SDL_OpenGamepad(ids[m_playerIndex]);
				SDL_free(ids);
			}
			return;
		}

		if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
		{
			if (m_gamepad && SDL_GetGamepadID(m_gamepad) == e.gdevice.which)
			{
				SDL_CloseGamepad(m_gamepad);
				m_gamepad = nullptr;
				m_currentButtons = 0;
			}
			return;
		}

	
		if (!m_gamepad || e.gbutton.which != SDL_GetGamepadID(m_gamepad))
			return;

		if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || e.type == SDL_EVENT_GAMEPAD_BUTTON_UP)
		{
			const unsigned int mask = SDLButtonToMask(static_cast<SDL_GamepadButton>(e.gbutton.button));
			if (mask == 0) return;

			if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
			{
				m_currentButtons |= mask;
				m_buttonsPressedThisFrame |= mask;
			}
			else
			{
				m_currentButtons &= ~mask;
				m_buttonsReleasedThisFrame |= mask;
			}
		}
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
	static unsigned int SDLButtonToMask(SDL_GamepadButton sdlBtn)
	{
		switch (sdlBtn)
		{
		case SDL_GAMEPAD_BUTTON_DPAD_UP:       return static_cast<unsigned int>(Controller::Button::DPadUp);
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN:     return static_cast<unsigned int>(Controller::Button::DPadDown);
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT:     return static_cast<unsigned int>(Controller::Button::DPadLeft);
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:    return static_cast<unsigned int>(Controller::Button::DPadRight);
		case SDL_GAMEPAD_BUTTON_START:         return static_cast<unsigned int>(Controller::Button::Start);
		case SDL_GAMEPAD_BUTTON_BACK:          return static_cast<unsigned int>(Controller::Button::Back);
		case SDL_GAMEPAD_BUTTON_LEFT_STICK:    return static_cast<unsigned int>(Controller::Button::LeftThumb);
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK:   return static_cast<unsigned int>(Controller::Button::RightThumb);
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return static_cast<unsigned int>(Controller::Button::LeftShoulder);
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:return static_cast<unsigned int>(Controller::Button::RightShoulder);
		case SDL_GAMEPAD_BUTTON_SOUTH:         return static_cast<unsigned int>(Controller::Button::ButtonA);
		case SDL_GAMEPAD_BUTTON_EAST:          return static_cast<unsigned int>(Controller::Button::ButtonB);
		case SDL_GAMEPAD_BUTTON_WEST:          return static_cast<unsigned int>(Controller::Button::ButtonX);
		case SDL_GAMEPAD_BUTTON_NORTH:         return static_cast<unsigned int>(Controller::Button::ButtonY);
		default:                               return 0;
		}
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
void dae::Controller::ProcessEvent(const SDL_Event& e) { m_pImpl->ProcessEvent(e); }
bool dae::Controller::IsDown(Button b) const { return m_pImpl->IsDown(b); }
bool dae::Controller::IsUp(Button b) const { return m_pImpl->IsUp(b); }
bool dae::Controller::IsPressed(Button b) const { return m_pImpl->IsPressed(b); }