#pragma once
#include <string>
#include <memory>
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
		~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

		void SetText(const std::string& text);
		const std::string& GetText() const { return m_text; }

	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}