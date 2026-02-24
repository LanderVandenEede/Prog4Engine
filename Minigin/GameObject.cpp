#include "GameObject.h"

dae::GameObject::~GameObject()
{
	if (m_parent)
		SetParent(nullptr);

	for (auto* child : m_children)
		child->m_parent = nullptr;
}

void dae::GameObject::Update(float deltaTime)
{
	for (const auto& component : m_components)
		component->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_components)
		component->Render();
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.f);
	SetWorldPositionDirty();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	m_transform.SetPosition(position);
	SetWorldPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition() const
{
	if (m_worldPositionDirty)
	{
		if (m_parent)
			m_worldPosition = m_parent->GetWorldPosition() + m_transform.GetPosition();
		else
			m_worldPosition = m_transform.GetPosition();

		m_worldPositionDirty = false;
	}
	return m_worldPosition;
}

void dae::GameObject::SetWorldPositionDirty()
{
	// No need to re-dirty if already dirty
	if (m_worldPositionDirty)
		return;

	m_worldPositionDirty = true;

	// Propagate dirty state down to all children
	for (auto* child : m_children)
		child->SetWorldPositionDirty();
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	if (newParent == this || newParent == m_parent)
		return;

	for (auto* child : m_children)
		if (child == newParent)
			return;

	if (m_parent)
	{
		auto& siblings = m_parent->m_children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	if (keepWorldPosition)
	{
		// Compute world position before the parent changes, then convert to
		// the new parent's local space so the object doesn't visually jump
		const glm::vec3 worldPos = GetWorldPosition();
		const glm::vec3 parentWorld = newParent ? newParent->GetWorldPosition() : glm::vec3{ 0.f };
		m_transform.SetPosition(worldPos - parentWorld);
	}

	m_parent = newParent;

	if (m_parent)
		m_parent->m_children.emplace_back(this);

	SetWorldPositionDirty();
}