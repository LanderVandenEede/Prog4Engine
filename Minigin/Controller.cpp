#include "Controller.h"

#ifndef __EMSCRIPTEN__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

static unsigned int ButtonToXInput(dae::Controller::Button btn)
{
	switch (btn)
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
	explicit Impl(unsigned int index)
		: m_index(index)
	{
		ZeroMemory(&m_lastState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&m_lastState, &m_state, sizeof(XINPUT_STATE));
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		XInputGetState(m_index, &m_state);

		const WORD changed = m_state.Gamepad.wButtons ^ m_lastState.Gamepad.wButtons;
		m_pressed = changed & m_state.Gamepad.wButtons;
		m_released = changed & ~m_state.Gamepad.wButtons;
	}

	bool IsDown(Button btn) const
	{
		return (m_state.Gamepad.wButtons & ButtonToXInput(btn)) != 0;
	}

	bool IsUp(Button btn) const
	{
		return (m_released & ButtonToXInput(btn)) != 0;
	}

	bool IsPressed(Button btn) const
	{
		return (m_pressed & ButtonToXInput(btn)) != 0;
	}

private:
	unsigned int m_index;
	XINPUT_STATE m_state{};
	XINPUT_STATE m_lastState{};
	WORD         m_pressed{};
	WORD         m_released{};
};

#else

#include <SDL3/SDL.h>

class dae::Controller::Impl
{
public:
	explicit Impl(unsigned int index)
		: m_index(index)
	{
		TryConnect();
	}

	~Impl()
	{
		if (m_handle)
			SDL_CloseGamepad(m_handle);
	}

	void Update()
	{
		
		if (!m_handle)
			TryConnect();

		m_last = m_current;
		m_current = 0;

		if (!m_handle)
			return;

		auto poll = [&](Controller::Button btn, SDL_GamepadButton sdlBtn)
			{
				if (SDL_GetGamepadButton(m_handle, sdlBtn))
					m_current |= static_cast<unsigned int>(btn);
			};

		poll(Controller::Button::DPadUp, SDL_GAMEPAD_BUTTON_DPAD_UP);
		poll(Controller::Button::DPadDown, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
		poll(Controller::Button::DPadLeft, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
		poll(Controller::Button::DPadRight, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
		poll(Controller::Button::Start, SDL_GAMEPAD_BUTTON_START);
		poll(Controller::Button::Back, SDL_GAMEPAD_BUTTON_BACK);
		poll(Controller::Button::LeftThumb, SDL_GAMEPAD_BUTTON_LEFT_STICK);
		poll(Controller::Button::RightThumb, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
		poll(Controller::Button::LeftShoulder, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
		poll(Controller::Button::RightShoulder, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
		poll(Controller::Button::ButtonA, SDL_GAMEPAD_BUTTON_SOUTH);
		poll(Controller::Button::ButtonB, SDL_GAMEPAD_BUTTON_EAST);
		poll(Controller::Button::ButtonX, SDL_GAMEPAD_BUTTON_WEST);
		poll(Controller::Button::ButtonY, SDL_GAMEPAD_BUTTON_NORTH);

		const unsigned int changed = m_current ^ m_last;
		m_pressed = changed & m_current;
		m_released = changed & ~m_current;
	}

	bool IsDown(Button btn) const
	{
		return (m_current & static_cast<unsigned int>(btn)) != 0;
	}

	bool IsUp(Button btn) const
	{
		return (m_released & static_cast<unsigned int>(btn)) != 0;
	}

	bool IsPressed(Button btn) const
	{
		return (m_pressed & static_cast<unsigned int>(btn)) != 0;
	}

private:
	void TryConnect()
	{
		int count = 0;
		SDL_JoystickID* available = SDL_GetGamepads(&count);
		if (available && m_index < static_cast<unsigned int>(count))
			m_handle = SDL_OpenGamepad(available[m_index]);
		SDL_free(available);
	}

	unsigned int m_index;
	SDL_Gamepad* m_handle{ nullptr };
	unsigned int m_current{};
	unsigned int m_last{};
	unsigned int m_pressed{};
	unsigned int m_released{};
};

#endif

dae::Controller::Controller(unsigned int index)
	: m_pImpl(std::make_unique<Impl>(index))
{
}


dae::Controller::~Controller() = default;

void dae::Controller::Update() { m_pImpl->Update(); }
bool dae::Controller::IsDown(Button b) const { return m_pImpl->IsDown(b); }
bool dae::Controller::IsUp(Button b) const { return m_pImpl->IsUp(b); }
bool dae::Controller::IsPressed(Button b) const { return m_pImpl->IsPressed(b); }