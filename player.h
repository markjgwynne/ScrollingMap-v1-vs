#pragma once

using namespace std;

#include "olcPixelGameEngine.h"

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

namespace ScrollingMap
{
	class Character
	{
	public:

		olc::vf2d *vfPosition;
		olc::vi2d *viTileSize;

		std::string sPlayerLocation;

		int iMovementSpeed = 10;

		Character(olc::vf2d *position, olc::vi2d *tileSize) {

			vfPosition = position;
			viTileSize = tileSize;

		}
		~Character() {
			//delete vfPosition;
			//delete viTileSize;
		}

		void Update(olc::PixelGameEngine* pge) {

			if (pge->GetKey(olc::Key::W).bPressed) vfPosition->y -= 1;
			if (pge->GetKey(olc::Key::S).bPressed) vfPosition->y += 1;
			if (pge->GetKey(olc::Key::A).bPressed) vfPosition->x -= 1;
			if (pge->GetKey(olc::Key::D).bPressed) vfPosition->x += 1;

			if (pge->GetKey(olc::Key::W).bHeld) vfPosition->y -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::S).bHeld) vfPosition->y += iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::A).bHeld) vfPosition->x -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::D).bHeld) vfPosition->x += iMovementSpeed * pge->GetElapsedTime();

			sPlayerLocation = "Player position, x: " + std::to_string(vfPosition->x) + ", y: " + std::to_string(vfPosition->y);

		}

		void Render(olc::PixelGameEngine* pge, olc::vf2d* vfCameraOffset) {

			// original
			//pge->FillRect(*vfPosition * *viTileSize, *viTileSize, olc::RED);
			
			// you get fractions of a position movement with button held using elapsed time. Change to integer to floor the number before multiplying by the tile size
			//pge->FillRect(olc::vi2d(*vfPosition) * *viTileSize, *viTileSize, olc::RED);
			
			//pge->FillRect((olc::vi2d(*vfCameraOffset) * *viTileSize), *viTileSize, olc::RED);

			pge->FillRect(std::floor(vfCameraOffset->x * viTileSize->x), std::floor(vfCameraOffset->y * viTileSize->y), viTileSize->x, viTileSize->y, olc::RED);

		}

	};
}

#endif