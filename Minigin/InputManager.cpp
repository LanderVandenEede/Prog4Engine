#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

dae::InputManager::InputManager()
{

	int numKeys{};
	SDL_GetKeyboardState(&numKeys); 
	m_previousKeyboardState.resize(numKeys, 0);
}

bool dae::InputManager::ProcessInput(float deltaTime)
{
	
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			return false;

		ImGui_ImplSDL3_ProcessEvent(&e);
		m_controller.ProcessEvent(e);
	}

	//Controller
	m_controller.Update();
	ProcessControllerCommands(deltaTime);

	//Keyboard state snapshot
	int numKeys{};
	const bool* currentKeyboardState = SDL_GetKeyboardState(&numKeys);
	ProcessKeyboardCommands(currentKeyboardState, deltaTime);

	//Save state for next frame
	m_previousKeyboardState.assign(currentKeyboardState, currentKeyboardState + numKeys);

	return true;
}

void dae::InputManager::ProcessKeyboardCommands(const bool* current, float deltaTime)
{
	for (const auto& [binding, command] : m_keyboardCommands)
	{
		const auto [key, state] = binding;
		const bool wasDown = m_previousKeyboardState[key];
		const bool isDown = current[key];

		switch (state)
		{
		case KeyState::Down:
			if (isDown) command->Execute(deltaTime);
			break;
		case KeyState::Up:
			if (!isDown && wasDown) command->Execute(deltaTime);
			break;
		case KeyState::Pressed:
			if (isDown && !wasDown) command->Execute(deltaTime);
			break;
		}
	}
}

void dae::InputManager::ProcessControllerCommands(float deltaTime)
{
	for (const auto& [binding, command] : m_controllerCommands)
	{
		const auto [button, state] = binding;

		switch (state)
		{
		case KeyState::Down:
			if (m_controller.IsDown(button)) command->Execute(deltaTime);
			break;
		case KeyState::Up:
			if (m_controller.IsUp(button)) command->Execute(deltaTime);
			break;
		case KeyState::Pressed:
			if (m_controller.IsPressed(button)) command->Execute(deltaTime);
			break;
		}
	}
}

void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command)
{
	m_keyboardCommands[{key, state}] = std::move(command);
}

void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode key, KeyState state)
{
	m_keyboardCommands.erase({ key, state });
}

void dae::InputManager::BindControllerCommand(Controller::Button button, KeyState state, std::unique_ptr<Command> command)
{
	m_controllerCommands[{button, state}] = std::move(command);
}

void dae::InputManager::UnbindControllerCommand(Controller::Button button, KeyState state)
{
	m_controllerCommands.erase({ button, state });
}