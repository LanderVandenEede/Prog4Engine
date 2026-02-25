#include "OrbitComponent.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <cmath>

dae::OrbitComponent::OrbitComponent(GameObject* owner, glm::vec2 center, float radius, float speed, bool clockwise)
	: Component(owner)
	, m_center(center)
	, m_radius(radius)
	, m_speed(speed)
	, m_clockwise(clockwise)
{
}

void dae::OrbitComponent::Update(float deltaTime)
{
	m_angle += m_clockwise ? -m_speed * deltaTime : m_speed * deltaTime;

	// Keep angle in [0, 2pi] to avoid float drift over time
	constexpr float twoPi = 6.28318530718f;
	if (m_angle > twoPi)
		m_angle -= twoPi;

	// Set local position as an offset from the center point
	// If the owner has a parent, this is relative to the parent's world position, so children automatically inherit the movement through GetWorldPosition()
	const float x = m_center.x + m_radius * std::cos(m_angle);
	const float y = m_center.y + m_radius * std::sin(m_angle);
	m_owner->SetLocalPosition(x, y);
}