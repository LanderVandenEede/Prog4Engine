#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class OrbitComponent final : public Component
	{
	public:
		// center:    local-space offset from the parent (or world point if no parent)
		// radius:    distance from the center point
		// speed:     radians per second
		// clockwise: true = clockwise, false = counterclockwise (default)
		OrbitComponent(GameObject* owner, glm::vec2 center, float radius, float speed, bool clockwise = false);
		~OrbitComponent() override = default;

		OrbitComponent(const OrbitComponent& other) = delete;
		OrbitComponent(OrbitComponent&& other) = delete;
		OrbitComponent& operator=(const OrbitComponent& other) = delete;
		OrbitComponent& operator=(OrbitComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
		glm::vec2 m_center;
		float m_radius;
		float m_speed;
		float m_angle{ 0.f };
		bool m_clockwise;
	};
}