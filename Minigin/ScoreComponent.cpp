#include "ScoreComponent.h"
#include "GameObject.h"

#ifdef USE_STEAMWORKS
#include <steam_api.h>
#endif

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::ScoreComponent::AddScore(int amount)
{
	m_score += amount;
	Notify(m_owner, GameEvent::PickupCollected);
}