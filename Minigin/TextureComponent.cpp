#include "TextureComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(GameObject* owner, const std::string& filename)
	: Component(owner)
	, m_texture(ResourceManager::GetInstance().LoadTexture(filename))
{
}

void dae::TextureComponent::Render() const
{
	if (m_texture)
	{
		const auto pos = m_owner->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}