#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class ScoreComponent;
	class SteamAchievementObserver final : public Component, public Observer
	{
	public:
		SteamAchievementObserver(GameObject* owner, ScoreComponent* scoreComponent);
		~SteamAchievementObserver() override;

		void OnNotify(GameObject* source, GameEvent event) override;
		void OnSubjectDestroyed(Subject* subject) override;

	private:
		ScoreComponent* m_scoreComponent;
		bool            m_achievementGranted{ false };
	};
}