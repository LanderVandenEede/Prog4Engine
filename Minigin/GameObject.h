#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject 
	{
	public:
		virtual void Update();
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);


#pragma region Components
		const Transform& GetTransform() const { return m_transform; }

		// Attach a component
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* raw = component.get();
			m_components.emplace_back(std::move(component));
			return raw;
		}

		// Retrieve the first component of type T, returns nullptr if not found
		template<typename T>
		T* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (auto* casted = dynamic_cast<T*>(component.get()))
					return casted;
			}
			return nullptr;
		}

#pragma endregion Components

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<Component>> m_components{};
	};
}
