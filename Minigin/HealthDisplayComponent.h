#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent;
	class TextComponent;

	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		HealthDisplayComponent(GameObject* owner, HealthComponent* healthComponent);
		~HealthDisplayComponent() override;

		void OnNotify(GameObject* source, GameEvent event) override;
		void OnSubjectDestroyed(Subject* subject) override;

	private:
		void UpdateText();

		HealthComponent* m_healthComponent;
		TextComponent* m_textComponent{ nullptr };
	};
}