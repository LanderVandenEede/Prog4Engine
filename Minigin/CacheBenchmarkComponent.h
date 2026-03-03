#pragma once
#include <vector>
#include "Component.h"

namespace dae
{
	class CacheBenchmarkComponent final : public Component
	{
	public:
		explicit CacheBenchmarkComponent(GameObject* owner);
		~CacheBenchmarkComponent() override = default;

		CacheBenchmarkComponent(const CacheBenchmarkComponent&) = delete;
		CacheBenchmarkComponent(CacheBenchmarkComponent&&) = delete;
		CacheBenchmarkComponent& operator=(const CacheBenchmarkComponent&) = delete;
		CacheBenchmarkComponent& operator=(CacheBenchmarkComponent&&) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

	private:
		struct Transform
		{
			float matrix[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		};

		struct GameObject3D
		{
			Transform transform;
			int ID{ 0 };
		};

		struct GameObject3DAlt
		{
			Transform* transform;
			int ID{ 0 };
		};

		void RunIntBenchmark();
		void RunGameObject3DBenchmark();
		void RunGameObject3DAltBenchmark();

		std::vector<float> m_intTimings{};
		std::vector<float> m_go3dTimings{};
		std::vector<float> m_go3dAltTimings{};
		std::vector<int> m_stepSizes{};

		mutable int m_sampleCount{ 10 };
		mutable bool m_intRunRequested{ false };
		mutable bool m_go3dRunRequested{ false };
		mutable bool m_go3dAltRunRequested{ false };
	};
}