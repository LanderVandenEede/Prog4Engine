#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		virtual void Update(float) {}
		virtual void Render() const {}

		void MarkForDelete() { m_markedForDeletion = true; }
		bool IsMarkedForDelete() const { return m_markedForDeletion; }

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		explicit Component(GameObject* owner);
		GameObject* m_owner;

	private:
		bool m_markedForDeletion{ false };
	};
}