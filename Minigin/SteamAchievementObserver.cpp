#include "SteamAchievementObserver.h"
#include "ScoreComponent.h"

#ifdef USE_STEAMWORKS
#include <steam_api.h>
#endif

dae::SteamAchievementObserver::SteamAchievementObserver(GameObject* owner, ScoreComponent* scoreComponent)
	: Component(owner)
	, m_scoreComponent(scoreComponent)
{
	m_scoreComponent->AddObserver(this);
}

dae::SteamAchievementObserver::~SteamAchievementObserver()
{
	if (m_scoreComponent)
		m_scoreComponent->RemoveObserver(this);
}

void dae::SteamAchievementObserver::OnNotify(GameObject*, GameEvent event)
{
	if (event != GameEvent::PickupCollected || !m_scoreComponent || m_achievementGranted)
		return;

#ifdef USE_STEAMWORKS
	if (m_scoreComponent->GetScore() >= 500)
	{
		m_achievementGranted = true;


		SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
		SteamUserStats()->StoreStats();

	}
#endif
}

void dae::SteamAchievementObserver::OnSubjectDestroyed(Subject* subject)
{
	if (subject == m_scoreComponent)
		m_scoreComponent = nullptr;
}