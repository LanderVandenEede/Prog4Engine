#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& position);
		const glm::vec3& GetLocalPosition() const { return m_transform.GetPosition(); }
		const glm::vec3& GetWorldPosition() const;

		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		GameObject* GetParent() const { return m_parent; }
		const std::vector<GameObject*>& GetChildren() const { return m_children; }

		void SetPosition(float x, float y) { SetLocalPosition(x, y); }

		const Transform& GetTransform() const { return m_transform; }

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

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void SetWorldPositionDirty();

		Transform m_transform{};
		mutable glm::vec3 m_worldPosition{};
		mutable bool m_worldPositionDirty{ true };

		std::vector<std::unique_ptr<Component>> m_components{};

		GameObject* m_parent{ nullptr };
		std::vector<GameObject*> m_children{};
	};
}