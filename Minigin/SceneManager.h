#pragma once
#include <vector>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void Update(float deltaTime);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}