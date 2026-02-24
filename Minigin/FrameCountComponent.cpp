#include "FrameCountComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

dae::FrameCountComponent::FrameCountComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::FrameCountComponent::Update(float deltaTime)
{
	// Lazy-fetch the TextComponent on first Update so it doesn't matter
	// which order the two components were added to the GameObject
	if (m_textComponent == nullptr)
	{
		m_textComponent = m_owner->GetComponent<TextComponent>();
		if (m_textComponent == nullptr)
			throw std::runtime_error("FrameCountComponent requires a TextComponent on the same GameObject");
	}

	m_accumulatedTime += deltaTime;
	++m_frameCount;

	if (m_accumulatedTime >= 1.f)
	{
		const float fps = m_frameCount / m_accumulatedTime;
		m_accumulatedTime -= 1.f;
		m_frameCount = 0;

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps;
		m_textComponent->SetText(oss.str() + " FPS");
	}
}