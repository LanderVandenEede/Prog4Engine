#pragma once
#include <memory>
#include <map>
#include <tuple>
#include <SDL3/SDL.h>
#include "Singleton.h"
#include "Command.h"
#include "Controller.h"
#include <vector>

namespace dae
{
	enum class KeyState { Down, Up, Pressed };

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		// Keyboard bindings (SDL_Scancode)
		void BindKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command);
		void UnbindKeyboardCommand(SDL_Scancode key, KeyState state);

		// Controller bindings (XInput)
		void BindControllerCommand(Controller::Button button, KeyState state, std::unique_ptr<Command> command);
		void UnbindControllerCommand(Controller::Button button, KeyState state);

	private:
		friend class Singleton<InputManager>;
		InputManager();

		void ProcessKeyboardCommands(const bool* keyboardState, float deltaTime);
		void ProcessControllerCommands(float deltaTime);

		using KeyboardBinding = std::tuple<SDL_Scancode, KeyState>;
		using ControllerBinding = std::tuple<Controller::Button, KeyState>;

		std::map<KeyboardBinding, std::unique_ptr<Command>> m_keyboardCommands{};
		std::map<ControllerBinding, std::unique_ptr<Command>> m_controllerCommands{};

		// Keyboard needs prev/current state to derive Up and Pressed
		std::vector<bool> m_previousKeyboardState{};

		Controller m_controller{ 0 };
	};
}