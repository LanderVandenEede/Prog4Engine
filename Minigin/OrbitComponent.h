#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class OrbitComponent final : public Component
	{
	public:
		// radius: distance from the owner's position
		// speed:  radians per second, negative = clockwise
		OrbitComponent(GameObject* owner, float radius, float speed);
		~OrbitComponent() override = default;

		OrbitComponent(const OrbitComponent&) = delete;
		OrbitComponent(OrbitComponent&&) = delete;
		OrbitComponent& operator=(const OrbitComponent&) = delete;
		OrbitComponent& operator=(OrbitComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		float m_radius;
		float m_speed;
		float m_angle{ 0.f };
		glm::vec3 m_center{};
	};
}