#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES

#include <math.h>
#include <array>
#include <ostream>
#include <sstream>
#include <iostream>

#include "olcPixelGameEngine.h"

#include "background.h"
#include "car.h"


namespace CarRace
{

	class Game : public olc::PixelGameEngine
	{
	public:
		Game()
		{
			sAppName = "Car Racing Game";
		}

	private:

#define PI 3.14159265

	public:

		GameBackground background;

		GameCar car;

		float fMovementIncrement = 100.0f;

		//olc::vf2d vfPlayerPos = {0.0f, 0.0f};
		olc::vf2d vfCameraPos = { 0.0f, 0.0f };

		bool OnUserCreate() override
		{

			background.LoadBackground(this, 8, 8);

			car.LoadCar(this, background.vBackgroundSize * 0.5f);

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{

			// HANDLE MOVEMENT AND COLLISION DETECTION

			olc::vf2d projectedPosition = car.ProjectMovement(this, fElapsedTime);

			car.vCarPos = projectedPosition;


			// RENDER SCREEN

			olc::vf2d vfCameraOffset = { float(ScreenWidth()) * 0.5f, float(ScreenHeight()) * 0.5f };
			vfCameraPos = car.vCarPos - vfCameraOffset;

			Clear(olc::DARK_GREEN);

			background.RenderBackground(this, vfCameraPos, ScreenWidth(), ScreenHeight());

			car.Render(this, vfCameraOffset);

			DrawString(1, 1, "Camera Pos, X: " + std::to_string(vfCameraPos.x) + ", Y: " + std::to_string(vfCameraPos.y) + " | Player Pos, X: " + std::to_string(car.vCarPos.x) + ", Y: " + std::to_string(car.vCarPos.y), olc::WHITE);

			return true;
		}
	};

}

int main()
{
	CarRace::Game demo;
	if (demo.Construct(800, 400, 2, 2))
		demo.Start();

	return 0;
}