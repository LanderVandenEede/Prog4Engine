#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);
		const Transform& GetTransform() const { return m_transform; }

#pragma region Components

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* raw = component.get();
			m_components.emplace_back(std::move(component));
			return raw;
		}

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

		template<typename T>
		bool RemoveComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
			const auto it = std::find_if(m_components.begin(), m_components.end(),
				[](const std::unique_ptr<Component>& component)
				{
					return dynamic_cast<T*>(component.get()) != nullptr;
				});

			if (it == m_components.end())
				return false;

			m_components.erase(it);
			return true;
		}

#pragma endregion Components

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<Component>> m_components{};
	};
}