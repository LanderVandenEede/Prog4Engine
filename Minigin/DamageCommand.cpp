#include "DamageCommand.h"
#include "HealthComponent.h"

dae::DamageCommand::DamageCommand(HealthComponent* healthComponent)
	: m_healthComponent(healthComponent)
{
	m_healthComponent->AddObserver(this);
}

dae::DamageCommand::~DamageCommand()
{
	if (m_healthComponent)
		m_healthComponent->RemoveObserver(this);
}

void dae::DamageCommand::Execute(float)
{
	if (!m_healthComponent)
		return;

	m_healthComponent->TakeDamage();
}

void dae::DamageCommand::OnSubjectDestroyed(Subject* subject)
{
	if (subject == m_healthComponent)
		m_healthComponent = nullptr;
}