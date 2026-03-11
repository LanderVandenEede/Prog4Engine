#include "MoveCommand.h"
#include "GameObject.h"

dae::MoveCommand::MoveCommand(GameObject* target, glm::vec3 direction, float speed)
	: m_target(target)
	, m_direction(direction)
	, m_speed(speed)
{
}

void dae::MoveCommand::Execute(float deltaTime)
{
	const glm::vec3 newPos = m_target->GetLocalPosition() + m_direction * m_speed * deltaTime;
	m_target->SetLocalPosition(newPos);
}