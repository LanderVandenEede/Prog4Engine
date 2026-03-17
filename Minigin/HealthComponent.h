#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class HealthComponent final : public Component, public Subject
	{
	public:
		HealthComponent(GameObject* owner, int startingLives);
		~HealthComponent() override = default;

		void TakeDamage();

		int GetLives() const { return m_lives; }

	private:
		int m_lives;
	};
}