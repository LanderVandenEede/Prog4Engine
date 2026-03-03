#include "CacheBenchmarkComponent.h"
#include <imgui.h>
#include <chrono>
#include <numeric>
#include <string>
#include <algorithm>

static constexpr int INT_BUFFER_SIZE = 1 << 26;
static constexpr int GO3D_BUFFER_SIZE = 1 << 22;

// Removes the lowest and highest sample then averages the rest.
// Falls back to a plain average if there are too few samples to trim.
static float TrimmedMean(std::vector<long long>& samples)
{
	if (samples.size() <= 2)
	{
		const long long sum = std::accumulate(samples.begin(), samples.end(), 0LL);
		return static_cast<float>(sum) / static_cast<float>(samples.size());
	}

	std::sort(samples.begin(), samples.end());

	// Trim one outlier from each end
	const long long sum = std::accumulate(samples.begin() + 1, samples.end() - 1, 0LL);
	return static_cast<float>(sum) / static_cast<float>(samples.size() - 2);
}

dae::CacheBenchmarkComponent::CacheBenchmarkComponent(GameObject* owner)
	: Component(owner)
{
	for (int step = 1; step <= 1024; step *= 2)
		m_stepSizes.emplace_back(step);
}

void dae::CacheBenchmarkComponent::RunIntBenchmark()
{
	const auto buffer = new int[INT_BUFFER_SIZE] {};

	m_intTimings.clear();
	m_intTimings.reserve(m_stepSizes.size());

	for (const int step : m_stepSizes)
	{
		std::vector<long long> samples{};
		samples.reserve(m_sampleCount);

		for (int sample = 0; sample < m_sampleCount; ++sample)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < INT_BUFFER_SIZE; i += step)
				buffer[i] *= 2;

			const auto end = std::chrono::high_resolution_clock::now();
			samples.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		m_intTimings.emplace_back(TrimmedMean(samples));
	}

	delete[] buffer;
	m_intRunRequested = false;
}

void dae::CacheBenchmarkComponent::RunGameObject3DBenchmark()
{
	// Buffer sized to match the int benchmark in number of elements
	const int go3dCount = GO3D_BUFFER_SIZE;
	const auto buffer = new GameObject3D[go3dCount]{};

	m_go3dTimings.clear();
	m_go3dTimings.reserve(m_stepSizes.size());

	for (const int step : m_stepSizes)
	{
		std::vector<long long> samples{};
		samples.reserve(m_sampleCount);

		for (int sample = 0; sample < m_sampleCount; ++sample)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < go3dCount; i += step)
				buffer[i].ID *= 2;

			const auto end = std::chrono::high_resolution_clock::now();
			samples.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		m_go3dTimings.emplace_back(TrimmedMean(samples));
	}

	delete[] buffer;
	m_go3dRunRequested = false;
}

void dae::CacheBenchmarkComponent::RunGameObject3DAltBenchmark()
{
	const int count = GO3D_BUFFER_SIZE;
	const auto objects = new GameObject3DAlt[count]{};

	// Each object needs its own transform allocated separately —
	// this is what makes the alt version cache-unfriendly: the transforms
	// are scattered across the heap rather than contiguous
	for (int i = 0; i < count; ++i)
		objects[i].transform = new Transform{};

	m_go3dAltTimings.clear();
	m_go3dAltTimings.reserve(m_stepSizes.size());

	for (const int step : m_stepSizes)
	{
		std::vector<long long> samples{};
		samples.reserve(m_sampleCount);

		for (int sample = 0; sample < m_sampleCount; ++sample)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < count; i += step)
				objects[i].ID *= 2;

			const auto end = std::chrono::high_resolution_clock::now();
			samples.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		m_go3dAltTimings.emplace_back(TrimmedMean(samples));
	}

	for (int i = 0; i < count; ++i)
		delete objects[i].transform;

	delete[] objects;
	m_go3dAltRunRequested = false;
}

void dae::CacheBenchmarkComponent::Update(float)
{
	if (m_intRunRequested)
		RunIntBenchmark();

	if (m_go3dRunRequested)
		RunGameObject3DBenchmark();

	if (m_go3dAltRunRequested)
		RunGameObject3DAltBenchmark();
}

void dae::CacheBenchmarkComponent::Render() const
{
	ImGui::Begin("Cache Benchmark");

	ImGui::InputInt("Samples", &m_sampleCount);
	if (m_sampleCount < 1) m_sampleCount = 1;

	// --- Exercise 1: int buffer ---
	ImGui::SeparatorText("Exercise 1 - int");
	if (ImGui::Button("Run##int"))
		m_intRunRequested = true;

	if (!m_intTimings.empty())
	{
		const std::string overlay = "avg us (" + std::to_string(m_sampleCount) + " samples)";
		ImGui::PlotLines("##int_timings",
			m_intTimings.data(),
			static_cast<int>(m_intTimings.size()),
			0, overlay.c_str(),
			0.f, *std::max_element(m_intTimings.begin(), m_intTimings.end()),
			ImVec2(0, 100));

		ImGui::Text("%-8s %s", "Step", "Avg (us)");
		for (size_t i = 0; i < m_stepSizes.size(); ++i)
			ImGui::Text("%-8d %.1f", m_stepSizes[i], m_intTimings[i]);
	}
	else
	{
		ImGui::TextDisabled("Press Run to benchmark...");
	}

	// --- Exercise 2: GameObject3D buffer ---
	ImGui::SeparatorText("Exercise 2 - GameObject3D");
	if (ImGui::Button("Run##go3d"))
		m_go3dRunRequested = true;

	if (!m_go3dTimings.empty())
	{
		const std::string overlay = "avg us (" + std::to_string(m_sampleCount) + " samples)";
		ImGui::PlotLines("##go3d_timings",
			m_go3dTimings.data(),
			static_cast<int>(m_go3dTimings.size()),
			0, overlay.c_str(),
			0.f, *std::max_element(m_go3dTimings.begin(), m_go3dTimings.end()),
			ImVec2(0, 100));

		ImGui::Text("%-8s %s", "Step", "Avg (us)");
		for (size_t i = 0; i < m_stepSizes.size(); ++i)
			ImGui::Text("%-8d %.1f", m_stepSizes[i], m_go3dTimings[i]);
	}
	else
	{
		ImGui::TextDisabled("Press Run to benchmark...");
	}

	// --- Exercise 3: GameObject3DAlt buffer (pointer to transform) ---
	ImGui::SeparatorText("Exercise 3 - GameObject3DAlt");
	if (ImGui::Button("Run##go3dalt"))
		m_go3dAltRunRequested = true;

	if (!m_go3dAltTimings.empty())
	{
		const std::string overlayAlt = "avg us (" + std::to_string(m_sampleCount) + " samples)";
		ImGui::PlotLines("##go3dalt_timings",
			m_go3dAltTimings.data(),
			static_cast<int>(m_go3dAltTimings.size()),
			0, overlayAlt.c_str(),
			0.f, *std::max_element(m_go3dAltTimings.begin(), m_go3dAltTimings.end()),
			ImVec2(0, 100));

		ImGui::Text("%-8s %s", "Step", "Avg (us)");
		for (size_t i = 0; i < m_stepSizes.size(); ++i)
			ImGui::Text("%-8d %.1f", m_stepSizes[i], m_go3dAltTimings[i]);
	}
	else
	{
		ImGui::TextDisabled("Press Run to benchmark...");
	}

	ImGui::End();
}