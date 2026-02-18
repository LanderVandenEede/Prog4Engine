#include "FrameCountComponent.h"
#include <sstream>
#include <iomanip>

dae::FrameCountComponent::FrameCountComponent(GameObject* owner, std::shared_ptr<Font> font)
	: TextComponent(owner, "0 FPS", std::move(font))
{
}

void dae::FrameCountComponent::Update(float deltaTime)
{
	m_accumulatedTime += deltaTime;
	++m_frameCount;

	if (m_accumulatedTime >= 0.25f)
	{
		const float fps = m_frameCount / m_accumulatedTime;
		m_accumulatedTime = 0.f;
		m_frameCount = 0;

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps;
		SetText(oss.str() + " FPS");
	}

	TextComponent::Update(deltaTime);
}