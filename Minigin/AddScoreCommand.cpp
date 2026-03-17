#include "AddScoreCommand.h"
#include "ScoreComponent.h"

dae::AddScoreCommand::AddScoreCommand(ScoreComponent* scoreComponent, int amount)
	: m_scoreComponent(scoreComponent)
	, m_amount(amount)
{
	m_scoreComponent->AddObserver(this);
}

dae::AddScoreCommand::~AddScoreCommand()
{
	if (m_scoreComponent)
		m_scoreComponent->RemoveObserver(this);
}

void dae::AddScoreCommand::Execute(float)
{
	if (!m_scoreComponent)
		return;

	m_scoreComponent->AddScore(m_amount);
}

void dae::AddScoreCommand::OnSubjectDestroyed(Subject* subject)
{
	if (subject == m_scoreComponent)
		m_scoreComponent = nullptr;
}