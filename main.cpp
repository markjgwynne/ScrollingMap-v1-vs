#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES

#include <math.h>
#include <array>
#include <ostream>
#include <sstream>
#include <iostream>

#include "olcPixelGameEngine.h"

#include "map.h"
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
				
		float fMovementIncrement = 100.0f;

		olc::vf2d vfPlayerPos = { 4.0f, 4.0f };
		olc::vf2d vfCameraPos = { 0.0f, 0.0f };
		
		olc::vi2d chunkCount = { 2, 2 };
		olc::vi2d tileCount = { 8, 8 };
		olc::vi2d tileSize = { 16, 16 };

		std::string sChunk[4] =
		{ 
			"..|....."
			"+++++++."
			"....|.+."
			"......+|"
			"..|...++"
			".....|.."
			"...|...."
			".|....|.",

			"..|...+."
			".|....+."
			"....|.+."
			".++++++|"
			"++..|..."
			".+...|.."
			".++.|..."
			".|+...|.",

			"..|....."
			"....|..."
			"...|.+++"
			".|..|+.|"
			".....+.."
			"++++++.."
			"..|..+.."
			".|...+.|",

			"..+..|.."
			"..+|...."
			"+++.|..."
			"..+++..|"
			"..|.+..."
			"....+|.."
			"..||++++"
			".|....|."

		};

		MapGenerator map;
		Character player = Character(&vfPlayerPos, &tileSize);


		bool OnUserCreate() override
		{

			map.GenerateChunks(&chunkCount, &tileCount, &tileSize, sChunk);

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{

			// HANDLE MOVEMENT AND COLLISION DETECTION
			
			player.Update(this);

			
			// RENDER SCREEN

			Clear(olc::WHITE);

			map.Render(this);

			player.Render(this);

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