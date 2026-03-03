#include <algorithm>
#include "Scene.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_objects)
		object->Update(deltaTime);

	// Delete marked components from all objects first
	for (auto& object : m_objects)
		object->DeleteMarked();

	// Detach marked objects from the hierarchy before destroying them
	for (auto& object : m_objects)
	{
		if (!object->IsMarkedForDelete())
			continue;

		// Detach children so they don't hold a dangling parent pointer
		const auto childrenCopy = object->GetChildren();
		for (auto* child : childrenCopy)
			child->SetParent(nullptr, false);

		// Detach from parent
		if (object->GetParent())
			object->SetParent(nullptr, false);
	}

	// Then remove any objects marked for deletion
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::unique_ptr<GameObject>& object)
			{
				return object->IsMarkedForDelete();
			}),
		m_objects.end()
	);
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
		object->Render();
}