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

		olc::vi2d *viPosition;
		olc::vi2d *viTileSize;
		olc::vi2d viNextPosition;

		std::string sPlayerLocation;

		int iMovementSpeed = 10;

		Character(olc::vi2d *position, olc::vi2d *tileSize) {

			viPosition = position;
			viNextPosition = *position;
			viTileSize = tileSize;

		}
		~Character() {
			//delete vfPosition;
			//delete viTileSize;
		}

		olc::vi2d* GetNextPosition(olc::PixelGameEngine* pge) {

			if (pge->GetKey(olc::Key::W).bPressed) viNextPosition.y -= 1;
			if (pge->GetKey(olc::Key::S).bPressed) viNextPosition.y += 1;
			if (pge->GetKey(olc::Key::A).bPressed) viNextPosition.x -= 1;
			if (pge->GetKey(olc::Key::D).bPressed) viNextPosition.x += 1;

			/*
			if (pge->GetKey(olc::Key::W).bHeld) viNextPosition.y -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::S).bHeld) viNextPosition.y += iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::A).bHeld) viNextPosition.x -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::D).bHeld) viNextPosition.x += iMovementSpeed * pge->GetElapsedTime();
			*/
			return &viNextPosition;
			
		}

		void MovePosition() {

			*viPosition = viNextPosition;
			sPlayerLocation = "Player position, x: " + std::to_string(viPosition->x) + ", y: " + std::to_string(viPosition->y);

		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			// original
			//pge->FillRect(*vfPosition * *viTileSize, *viTileSize, olc::RED);
			
			// you get fractions of a position movement with button held using elapsed time. Change to integer to floor the number before multiplying by the tile size
			//pge->FillRect(olc::vi2d(*vfPosition) * *viTileSize, *viTileSize, olc::RED);
			
			pge->FillRect(*viCameraOffset * *viTileSize, *viTileSize, olc::RED);

			//olc::vi2d position = { (int)std::floor(vfCameraOffset->x) * viTileSize->x ,(int)std::floor(vfCameraOffset->y) * viTileSize->y) };

			//pge->FillRect(position.x, position.y, viTileSize->x, viTileSize->y, olc::RED);

		}

	};
}

#endif