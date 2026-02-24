#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;

	class FrameCountComponent final : public Component
	{
	public:
		explicit FrameCountComponent(GameObject* owner);
		~FrameCountComponent() override = default;

		FrameCountComponent(const FrameCountComponent& other) = delete;
		FrameCountComponent(FrameCountComponent&& other) = delete;
		FrameCountComponent& operator=(const FrameCountComponent& other) = delete;
		FrameCountComponent& operator=(FrameCountComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
		float m_accumulatedTime{ 0.f };
		int m_frameCount{ 0 };
		TextComponent* m_textComponent{ nullptr };
	};
}