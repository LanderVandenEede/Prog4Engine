#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		explicit Component(GameObject* owner);
		virtual ~Component() = default;

		virtual void Update(float) {}
		virtual void Render() const {}

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		GameObject* m_owner;
	};
}