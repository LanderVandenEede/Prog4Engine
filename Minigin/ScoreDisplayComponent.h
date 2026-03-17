#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class ScoreComponent;
	class TextComponent;

	class ScoreDisplayComponent final : public Component, public Observer
	{
	public:
		ScoreDisplayComponent(GameObject* owner, ScoreComponent* scoreComponent);
		~ScoreDisplayComponent() override;

		void OnNotify(GameObject* source, GameEvent event) override;
		void OnSubjectDestroyed(Subject* subject) override;

	private:
		void UpdateText();

		ScoreComponent* m_scoreComponent;
		TextComponent* m_textComponent{ nullptr };
	};
}