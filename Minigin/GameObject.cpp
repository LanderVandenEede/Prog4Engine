#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (const auto& component : m_components)
		component->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	if (m_texture)
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);

	for (const auto& component : m_components)
		component->Render();
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}