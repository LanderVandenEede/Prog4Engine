#include "HealthDisplayComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner, HealthComponent* healthComponent)
	: Component(owner)
	, m_healthComponent(healthComponent)
{
	m_healthComponent->AddObserver(this);
}

dae::HealthDisplayComponent::~HealthDisplayComponent()
{
	if (m_healthComponent)
		m_healthComponent->RemoveObserver(this);
}

void dae::HealthDisplayComponent::OnNotify(GameObject*, GameEvent event)
{
	if (event == GameEvent::PlayerHit || event == GameEvent::PlayerDied)
		UpdateText();
}

void dae::HealthDisplayComponent::OnSubjectDestroyed(Subject* subject)
{
	if (subject == m_healthComponent)
		m_healthComponent = nullptr;
}

void dae::HealthDisplayComponent::UpdateText()
{
	if (!m_textComponent)
		m_textComponent = m_owner->GetComponent<TextComponent>();

	if (m_textComponent && m_healthComponent)
		m_textComponent->SetText("# lives: " + std::to_string(m_healthComponent->GetLives()));
}