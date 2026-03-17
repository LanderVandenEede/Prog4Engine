#pragma once
#include "Command.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent;

	class DamageCommand final : public Command, public Observer
	{
	public:
		explicit DamageCommand(HealthComponent* healthComponent);
		~DamageCommand() override;

		void Execute(float deltaTime) override;
		void OnNotify(GameObject*, GameEvent) override {}
		void OnSubjectDestroyed(Subject* subject) override;

	private:
		HealthComponent* m_healthComponent;
	};
}