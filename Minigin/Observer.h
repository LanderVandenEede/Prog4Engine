#pragma once
#include "GameEvent.h"

namespace dae
{
	class GameObject;
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* source, GameEvent event) = 0;

		virtual void OnSubjectDestroyed(Subject*) {}

	protected:
		Observer() = default;
	};
}