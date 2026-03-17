#pragma once
#include <vector>
#include "GameEvent.h"

namespace dae
{
	class Observer;
	class GameObject;

	class Subject
	{
	public:
		virtual ~Subject();

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		// Pushes an event into the EventQueue
		void Notify(GameObject* source, GameEvent event);

	private:
		std::vector<Observer*> m_observers{};
	};
}