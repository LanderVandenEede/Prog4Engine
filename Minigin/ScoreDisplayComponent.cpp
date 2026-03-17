#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, ScoreComponent* scoreComponent)
	: Component(owner)
	, m_scoreComponent(scoreComponent)
{
	m_scoreComponent->AddObserver(this);
}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if (m_scoreComponent)
		m_scoreComponent->RemoveObserver(this);
}

void dae::ScoreDisplayComponent::OnNotify(GameObject*, GameEvent event)
{
	if (event == GameEvent::PickupCollected)
		UpdateText();
}

void dae::ScoreDisplayComponent::OnSubjectDestroyed(Subject* subject)
{
	if (subject == m_scoreComponent)
		m_scoreComponent = nullptr;
}

void dae::ScoreDisplayComponent::UpdateText()
{
	if (!m_textComponent)
		m_textComponent = m_owner->GetComponent<TextComponent>();

	if (m_textComponent && m_scoreComponent)
		m_textComponent->SetText("Score: " + std::to_string(m_scoreComponent->GetScore()));
}