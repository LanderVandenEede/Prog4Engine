#include "Component.h"
#include "GameObject.h"

namespace dae
{
	Component::Component(GameObject* owner)
		: m_owner(owner)
	{
	}
}
