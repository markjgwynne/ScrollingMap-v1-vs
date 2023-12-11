#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES

#include <math.h>
#include <array>
#include <ostream>
#include <sstream>
#include <iostream>

#include "olcPixelGameEngine.h"

#include "world.h"
#include "player.h"


namespace ScrollingMap
{
	class Game : public olc::PixelGameEngine
	{
	private:
		#define PI 3.14159265

	public:
		Game()
		{
			sAppName = "Scrolling Map";
		}

		~Game()
		{
			
		}
				
		float fMovementIncrement = 100.0f;

		olc::vf2d vfPlayerPos = { 20.0f, 4.0f };
		olc::vf2d vfCameraPos = { 0.0f, 0.0f };
		
		olc::vi2d chunkCount = { 4, 4 };
		olc::vi2d tileCount = { 8, 8 };
		olc::vi2d tileSize = { 16, 16 };

		GameWorld world = GameWorld(&chunkCount, &tileCount, &tileSize);
		Character player = Character(&vfPlayerPos, &tileSize);

		bool OnUserCreate() override
		{

			world.GenerateChunks(&vfPlayerPos);

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{

			Clear(olc::WHITE);

			// HANDLE MOVEMENT AND COLLISION DETECTION
			
			player.Update(this);

			world.Update(this, &vfPlayerPos);

			// RENDER SCREEN	
			
			//Clear(olc::WHITE);

			world.Render(this);

			player.Render(this);

			DrawString(1, 1, player.sPlayerLocation, olc::BLACK);
			DrawString(1, 11, world.sChunkLocation, olc::BLACK);

			return true;
		}
	};
}

int main()
{
	ScrollingMap::Game demo;
	if (demo.Construct(800, 400, 2, 2))
		demo.Start();

	return 0;
}