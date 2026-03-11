#pragma once
#include <memory>

namespace dae
{
	class Controller final
	{
	public:
		enum class Button : unsigned int
		{
			DPadUp = 1 << 0,
			DPadDown = 1 << 1,
			DPadLeft = 1 << 2,
			DPadRight = 1 << 3,
			Start = 1 << 4,
			Back = 1 << 5,
			LeftThumb = 1 << 6,
			RightThumb = 1 << 7,
			LeftShoulder = 1 << 8,
			RightShoulder = 1 << 9,
			ButtonA = 1 << 10,
			ButtonB = 1 << 11,
			ButtonX = 1 << 12,
			ButtonY = 1 << 13,
		};

		explicit Controller(unsigned int playerIndex);
		~Controller();

		void Update();

		bool IsDown(Button button) const;
		bool IsUp(Button button) const;
		bool IsPressed(Button button) const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}