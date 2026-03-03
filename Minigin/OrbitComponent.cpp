#include "OrbitComponent.h"
#include "GameObject.h"
#include <cmath>

dae::OrbitComponent::OrbitComponent(GameObject* owner, float radius, float speed)
	: Component(owner)
	, m_radius(radius)
	, m_speed(speed)
	, m_center(owner->GetLocalPosition())
{
}

void dae::OrbitComponent::Update(float deltaTime)
{
	m_angle += m_speed * deltaTime;

	constexpr float twoPi = 6.28318530718f;
	if (m_angle > twoPi)  m_angle -= twoPi;
	if (m_angle < -twoPi) m_angle += twoPi;

	const float x = m_center.x + m_radius * std::cos(m_angle);
	const float y = m_center.y + m_radius * std::sin(m_angle);
	m_owner->SetLocalPosition(x, y);
}