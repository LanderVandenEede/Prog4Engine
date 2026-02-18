#pragma once
#include "TextComponent.h"

namespace dae
{
	class Font;

	class FrameCountComponent final : public TextComponent
	{
	public:
		FrameCountComponent(GameObject* owner, std::shared_ptr<Font> font);
		~FrameCountComponent() override = default;

		FrameCountComponent(const FrameCountComponent& other) = delete;
		FrameCountComponent(FrameCountComponent&& other) = delete;
		FrameCountComponent& operator=(const FrameCountComponent& other) = delete;
		FrameCountComponent& operator=(FrameCountComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
		float m_accumulatedTime{ 0.f };
		int m_frameCount{ 0 };
	};
}
