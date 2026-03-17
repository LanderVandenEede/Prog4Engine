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
#include "MoveCommand.h"
#include "DamageCommand.h"
#include "AddScoreCommand.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreDisplayComponent.h"
#include "SteamAchievementObserver.h"

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
	CtrlInputObj->AddComponent<dae::TextComponent>("Use the D-Pad to move pink pengo, A to add score, X to inflict damage", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(CtrlInputObj));

	//KeyboardHelp
	auto KBInputObj = std::make_unique<dae::GameObject>();
	KBInputObj->SetPosition(10, 110);
	KBInputObj->AddComponent<dae::TextComponent>("Use WASD to move blue pengo, Z to add score, C to inflict damage", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(KBInputObj));

	auto& input = dae::InputManager::GetInstance();

	//Character 1: WASD
	constexpr float speed1 = 100.f;
	auto character1 = std::make_unique<dae::GameObject>();
	character1->SetPosition(300, 300);
	character1->AddComponent<dae::TextureComponent>("GameSprite.png");
	auto* health1 = character1->AddComponent<dae::HealthComponent>(3);
	auto* score1 = character1->AddComponent<dae::ScoreComponent>();
	dae::GameObject* char1Ptr = character1.get();
	scene.Add(std::move(character1));

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 0, -1, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 0,  1, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ -1,  0, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char1Ptr, glm::vec3{ 1,  0, 0 }, speed1));
	input.BindKeyboardCommand(SDL_SCANCODE_C, dae::KeyState::Pressed, std::make_unique<dae::DamageCommand>(health1));
	input.BindKeyboardCommand(SDL_SCANCODE_Z, dae::KeyState::Pressed, std::make_unique<dae::AddScoreCommand>(score1, 100));

	auto lives1Obj = std::make_unique<dae::GameObject>();
	lives1Obj->SetPosition(10, 140);
	lives1Obj->AddComponent<dae::TextComponent>("# lives: 3", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	lives1Obj->AddComponent<dae::HealthDisplayComponent>(health1);
	scene.Add(std::move(lives1Obj));

	auto score1Obj = std::make_unique<dae::GameObject>();
	score1Obj->SetPosition(10, 165);
	score1Obj->AddComponent<dae::TextComponent>("Score: 0", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	score1Obj->AddComponent<dae::ScoreDisplayComponent>(score1);
	scene.Add(std::move(score1Obj));

	auto achievement1Obj = std::make_unique<dae::GameObject>();
	achievement1Obj->AddComponent<dae::SteamAchievementObserver>(score1);
	scene.Add(std::move(achievement1Obj));

	//Character 2: DPad
	constexpr float speed2 = speed1 * 2.f;
	auto character2 = std::make_unique<dae::GameObject>();
	character2->SetPosition(500, 300);
	character2->AddComponent<dae::TextureComponent>("GameSprite_01.png");
	auto* health2 = character2->AddComponent<dae::HealthComponent>(3);
	auto* score2 = character2->AddComponent<dae::ScoreComponent>();
	dae::GameObject* char2Ptr = character2.get();
	scene.Add(std::move(character2));

	input.BindControllerCommand(dae::Controller::Button::DPadUp, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 0, -1, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadDown, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 0,  1, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadLeft, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ -1,  0, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::DPadRight, dae::KeyState::Down, std::make_unique<dae::MoveCommand>(char2Ptr, glm::vec3{ 1,  0, 0 }, speed2));
	input.BindControllerCommand(dae::Controller::Button::ButtonX, dae::KeyState::Pressed, std::make_unique<dae::DamageCommand>(health2));
	input.BindControllerCommand(dae::Controller::Button::ButtonA, dae::KeyState::Pressed, std::make_unique<dae::AddScoreCommand>(score2, 100));

	auto lives2Obj = std::make_unique<dae::GameObject>();
	lives2Obj->SetPosition(10, 195);
	lives2Obj->AddComponent<dae::TextComponent>("# lives: 3", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	lives2Obj->AddComponent<dae::HealthDisplayComponent>(health2);
	scene.Add(std::move(lives2Obj));

	auto score2Obj = std::make_unique<dae::GameObject>();
	score2Obj->SetPosition(10, 220);
	score2Obj->AddComponent<dae::TextComponent>("Score: 0", fontSmall, SDL_Color{ 255, 255, 255, 255 });
	score2Obj->AddComponent<dae::ScoreDisplayComponent>(score2);
	scene.Add(std::move(score2Obj));

	auto achievement2Obj = std::make_unique<dae::GameObject>();
	achievement2Obj->AddComponent<dae::SteamAchievementObserver>(score2);
	scene.Add(std::move(achievement2Obj));
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