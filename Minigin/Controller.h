#pragma once
#include <memory>

union SDL_Event;

namespace dae
{
	class Controller final
	{
	public:
		enum class Button : unsigned int
		{
			DPadUp,
			DPadDown,
			DPadLeft,
			DPadRight,
			Start,
			Back,
			LeftThumb,
			RightThumb,
			LeftShoulder,
			RightShoulder,
			ButtonA,
			ButtonB,
			ButtonX,
			ButtonY,
		};

		explicit Controller(unsigned int playerIndex);
		~Controller();

		void Update();
		void ProcessEvent(const SDL_Event& e);

		bool IsDown(Button button) const;
		bool IsUp(Button button) const;
		bool IsPressed(Button button) const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}