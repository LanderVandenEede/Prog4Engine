#include "Subject.h"
#include "Observer.h"
#include "EventQueue.h"
#include <algorithm>

dae::Subject::~Subject()
{
	for (Observer* observer : m_observers)
		observer->OnSubjectDestroyed(this);
}

void dae::Subject::AddObserver(Observer* observer)
{
	m_observers.push_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	m_observers.erase(
		std::remove(m_observers.begin(), m_observers.end(), observer),
		m_observers.end()
	);
}

void dae::Subject::Notify(GameObject* source, GameEvent event)
{
	EventQueue::GetInstance().Push(source, event, m_observers);
}