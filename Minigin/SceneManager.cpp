#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>

void dae::SceneManager::Update(float deltaTime)
{
	for (auto& scene : m_scenes)
		scene->Update(deltaTime);

	// Remove scenes marked for deletion after all updates
	m_scenes.erase(
		std::remove_if(m_scenes.begin(), m_scenes.end(),
			[](const std::unique_ptr<Scene>& scene)
			{
				return scene->IsMarkedForDelete();
			}),
		m_scenes.end()
	);
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_scenes)
		scene->Render();
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}