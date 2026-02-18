#pragma once
#include <string>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update(float) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		const std::string& GetText() const { return m_text; }

	private:
		bool m_needsUpdate;
		std::string m_text;
		SDL_Color m_color;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
