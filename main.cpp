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

		/*
			Positions are based on a grid of tileSize x and y size blocks.
			Positions are calculated from the grid positions and only converted to world space using tileSize on render.
			Any assets or positions that are drived from the screen location need to be divided by the tileSize to obtain the grid position.
		*/

		olc::vi2d viPlayerPos = { 10, 10 };
		olc::vi2d viCameraOffset = { 0, 0 };
		
		olc::vi2d chunkCount = { 4, 4 };
		olc::vi2d tileCount = { 16, 16 };
		olc::vi2d tileSize = { 16, 16 };
		olc::vi2d renderDistance = { 1, 1 };

		bool useSprites = false;

		GameWorld world = GameWorld(&chunkCount, &tileCount, &tileSize, &renderDistance);
		Character player = Character(&viPlayerPos, &tileSize);

		olc::Sprite* sprTest = nullptr;

		bool OnUserCreate() override
		{
			world.GenerateWorld(123, &viPlayerPos);

			if (useSprites) {
				player.GenerateSprite(this);
				world.GenerateSprite(this);
			}
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{

			// clear is placed here to ensure that any debugging information rendered during the update functions are still visible.
			Clear(olc::WHITE);

			// HANDLE MOVEMENT AND COLLISION DETECTION
			
			if (world.UpdatePlayerPosition(this, player.GetNextPosition(this)) == true) {
				player.SetNextPosition();
			};
			
			// RENDER SCREEN

			// offset is used as the position of the player
			// convert to int to ensure movement is tile by tile and not fractions of a tile.
			viCameraOffset = { (int)((ScreenWidth() / tileSize.x) * 0.5f), (int)((ScreenHeight() / tileSize.y) * 0.5f) };

			// take the vfPlayerPos from the cameraoffset (centre screen) to locate the world position that marks the start of the screen (0, 0)
			// while doing this, convert to an integer vector to floor the numbers to the lowest round number.
			// This makes sure the movement is tile by tile and not fractions of a tile.
			
			olc::vi2d viMapPosition;

			if (useSprites) {
				viMapPosition = { viPlayerPos - viCameraOffset }; // sprite rendering version
				world.RenderSprite(this, &viMapPosition);
				player.RenderSprite(this, &viCameraOffset);
			} else {
				viMapPosition = { viCameraOffset - viPlayerPos }; // pixel rendering version
				world.Render(this, &viMapPosition);
				player.Render(this, &viCameraOffset);
			}

			int sY = 1, textHeight = 10;

			DrawString(1, sY, player.sPlayerLocation, olc::BLACK);
			if (useSprites) {
				DrawString(1, sY += textHeight, world.sSpriteRenderLocation, olc::BLACK);
			}
			DrawString(1, sY += textHeight, world.sChunkLocation, olc::BLACK);
			DrawString(1, sY += textHeight, world.sTileLocation, olc::BLACK);
			DrawString(1, sY += textHeight, world.sTileAwareness, olc::BLACK);
			
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