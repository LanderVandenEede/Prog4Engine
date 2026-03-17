#include "EventQueue.h"
#include "Observer.h"

void dae::EventQueue::Push(GameObject* source, GameEvent event, const std::vector<Observer*>& observers)
{
	m_queue.push_back({ source, event, observers });
}

void dae::EventQueue::Drain()
{
	std::vector<QueuedEvent> toProcess;
	toProcess.swap(m_queue);

	for (const auto& queued : toProcess)
		for (Observer* observer : queued.observers)
			observer->OnNotify(queued.source, queued.event);
}