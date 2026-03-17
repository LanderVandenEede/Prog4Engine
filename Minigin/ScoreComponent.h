#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent final : public Component, public Subject
	{
	public:
		explicit ScoreComponent(GameObject* owner);
		~ScoreComponent() override = default;

		void AddScore(int amount);

		int GetScore() const { return m_score; }

	private:
		int  m_score{ 0 };
	};
}