#pragma once
#include <vector>
#include "Singleton.h"
#include "GameEvent.h"

namespace dae
{
	class Observer;
	class GameObject;

	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		// Adds an event to the back of the queue
		void Push(GameObject* source, GameEvent event, const std::vector<Observer*>& observers);

		// Dispatches all queued events to their observers in the order they were pushed, then clears the queue
		void Drain();

	private:
		friend class Singleton<EventQueue>;
		EventQueue() = default;

		struct QueuedEvent
		{
			GameObject* source;
			GameEvent event;
			std::vector<Observer*> observers;
		};

		std::vector<QueuedEvent> m_queue{};
	};
}