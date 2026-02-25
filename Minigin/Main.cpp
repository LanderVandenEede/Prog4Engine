#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "FrameCountComponent.h"
#include "OrbitComponent.h"
#include "TextureComponent.h"

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

	// Parent object orbit
	auto orbitParent = std::make_unique<dae::GameObject>();
	orbitParent->AddComponent<dae::TextureComponent>("GameSprite.png");
	orbitParent->AddComponent<dae::OrbitComponent>(glm::vec2{ 300.f, 300.f }, 20.f, 7.f, true);

	// Child object orbiting around the parent
	auto orbitChild = std::make_unique<dae::GameObject>();
	orbitChild->AddComponent<dae::TextureComponent>("GameSprite.png");
	orbitChild->AddComponent<dae::OrbitComponent>(glm::vec2{ 0.f, 0.f }, 40.f, 4.f);
	orbitChild->SetParent(orbitParent.get(), false);


	scene.Add(std::move(orbitParent));
	scene.Add(std::move(orbitChild));


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