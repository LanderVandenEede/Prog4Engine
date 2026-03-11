#pragma once
#include <glm/glm.hpp>
#include "Command.h"

namespace dae
{
	class GameObject;

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* target, glm::vec3 direction, float speed);
		void Execute(float deltaTime) override;

	private:
		GameObject* m_target;
		glm::vec3 m_direction;
		float m_speed;
	};
}