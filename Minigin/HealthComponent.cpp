#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int startingLives)
	: Component(owner)
	, m_lives(startingLives)
{
}

void dae::HealthComponent::TakeDamage()
{
	if (m_lives <= 0)
		return;

	--m_lives;

	if (m_lives <= 0)
	{
		// The player has run out of lives
		Notify(m_owner, GameEvent::PlayerHit);
		Notify(m_owner, GameEvent::PlayerDied);
		m_owner->MarkForDelete();
	}
	else
	{
		// The player took a hit but is still alive.
		Notify(m_owner, GameEvent::PlayerHit);
	}
}