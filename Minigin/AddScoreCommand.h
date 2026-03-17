#pragma once
#include "Command.h"
#include "Observer.h"

namespace dae
{
	class ScoreComponent;

	class AddScoreCommand final : public Command, public Observer
	{
	public:
		AddScoreCommand(ScoreComponent* scoreComponent, int amount);
		~AddScoreCommand() override;

		void Execute(float deltaTime) override;
		void OnNotify(GameObject*, GameEvent) override {}
		void OnSubjectDestroyed(Subject* subject) override;

	private:
		ScoreComponent* m_scoreComponent;
		int             m_amount;
	};
}