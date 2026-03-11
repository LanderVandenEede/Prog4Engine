#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FrameCountComponent.h"
#include "OrbitComponent.h"
#include "CacheBenchmarkComponent.h"
#include "MoveCommand.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("background.png");
	scene.Add(std::move(go));

	//Dae Logo
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(358, 180);
	go->AddComponent<dae::TextureComponent>("logo.png");
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//Title
	auto textObj = std::make_unique<dae::GameObject>();
	textObj->SetPosition(292, 20);
	textObj->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(textObj));

	//FPS Counter
	auto frameObj = std::make_unique<dae::GameObject>();
	frameObj->SetPosition(10, 10);
	frameObj->AddComponent<dae::TextComponent>("0.0 FPS", font);
	frameObj->AddComponent<dae::FrameCountComponent>();
	scene.Add(std::move(frameObj));


	//// Orbit objects
	//auto orbitParent = std::make_unique<dae::GameObject>();
	//orbitParent->SetPosition(300.f, 300.f);
	//orbitParent->AddComponent<dae::TextureComponent>("GameSprite.png");
	//orbitParent->AddComponent<dae::OrbitComponent>(10.f, 5.f);
	//dae::GameObject* orbitParentPtr = orbitParent.get();
	//
	//auto orbitChild = std::make_unique<dae::GameObject>();
	//orbitChild->AddComponent<dae::TextureComponent>("GameSprite.png");
	//orbitChild->AddComponent<dae::OrbitComponent>(40.f, -3.f);
	//dae::GameObject* orbitChildPtr = orbitChild.get();
	//orbitChildPtr->SetParent(orbitParentPtr, false);
	//
	//scene.Add(std::move(orbitChild));
	//scene.Add(std::move(orbitParent));


	//// Cache GUI
	//auto benchmarkObj = std::make_unique<dae::GameObject>();
	//benchmarkObj->AddComponent<dae::CacheBenchmarkComponent>();
	//scene.Add(std::move(benchmarkObj));


	//ControllerHelp
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto CtrlInputObj = std::make_unique<dae::GameObject>();
	CtrlInputObj->SetPosition(10, 80);
	CtrlInputObj->AddComponent<dae::TextComponent>("Use the D-Pad to move pink pengo", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(CtrlInputObj));

	//KeyboardHelp
	auto KBInputObj = std::make_unique<dae::GameObject>();
	KBInputObj->SetPosition(10, 110);
	KBInputObj->AddComponent<dae::TextComponent>("Use WASD to move blue pengo", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(KBInputObj));

	//Character 1: WASD
	constexpr float speed1 = 100.f;
	auto character1 = std::make_unique<dae::GameObject>();
	character1->SetPosition(300, 300);
	character1->AddComponent<dae::TextureComponent>("GameSprite.png");
	dae::GameObject* char1Ptr = character1.get();
	scene.Add(std::move(character1));

	auto& input = dae::InputManager::GetInstance();
	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 0, -1, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 0,  1, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ -1,  0, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 1,  0, 0 }, speed1));

	//Character 2: DPad
	constexpr float speed2 = speed1 * 2.f;
	auto character2 = std::make_unique<dae::GameObject>();
	character2->SetPosition(500, 300);
	character2->AddComponent<dae::TextureComponent>("GameSprite_01.png");
	dae::GameObject* char2Ptr = character2.get();
	scene.Add(std::move(character2));

	input.BindControllerCommand(dae::Controller::Button::DPadUp, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 0, -1, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadDown, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 0,  1, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadLeft, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ -1,  0, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadRight, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 1,  0, 0 }, speed2));
}

int main(int, char* []) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}