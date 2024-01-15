#pragma once

using namespace std;

#include "sharedUtilities.h"
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

		olc::vf2d vfHeldMovement;

		xDirection xAxisDirection;
		yDirection yAxisDirection;

		std::string sPlayerLocation;

		int iMovementSpeed = 10;

		olc::Sprite* playerSprite = nullptr;
		olc::Decal* playerDecal = nullptr;

		Character(olc::vi2d *position, olc::vi2d *tileSize) {

			viPosition = position;
			viNextPosition = *position;
			viTileSize = tileSize;
			xAxisDirection = xDirection::NONE_X;
			yAxisDirection = yDirection::NONE_Y;

		}
		~Character() {
			//delete vfPosition;
			//delete viTileSize;
		}

		olc::vi2d* GetNextPosition(olc::PixelGameEngine* pge) {

			viNextPosition = *viPosition;

			if (pge->GetKey(olc::Key::W).bPressed) viNextPosition.y -= 1;
			if (pge->GetKey(olc::Key::S).bPressed) viNextPosition.y += 1;
			if (pge->GetKey(olc::Key::A).bPressed) viNextPosition.x -= 1;
			if (pge->GetKey(olc::Key::D).bPressed) viNextPosition.x += 1;

			if (pge->GetKey(olc::Key::W).bHeld) vfHeldMovement.y -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::S).bHeld) vfHeldMovement.y += iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::A).bHeld) vfHeldMovement.x -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::D).bHeld) vfHeldMovement.x += iMovementSpeed * pge->GetElapsedTime();
			
			if (vfHeldMovement.y > 1 || vfHeldMovement.y < -1) { viNextPosition.y += (int)vfHeldMovement.y; vfHeldMovement.y = 0; }
			if (vfHeldMovement.x > 1 || vfHeldMovement.x < -1) { viNextPosition.x += (int)vfHeldMovement.x; vfHeldMovement.x = 0; }

			return &viNextPosition;
			
		}

		void SetNextPosition() {
			*viPosition = viNextPosition;
			sPlayerLocation = "Player position, x: " + std::to_string(viPosition->x) + ", y: " + std::to_string(viPosition->y);
		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			pge->FillRect(*viCameraOffset * *viTileSize, *viTileSize, olc::RED);

		}

		void GenerateSprite(olc::PixelGameEngine* pge) {

			playerSprite = new olc::Sprite(viTileSize->x, viTileSize->y);

			pge->SetDrawTarget(playerSprite);

			pge->FillRect({ 0, 0 }, *viTileSize, olc::RED);

			pge->FillRect({ 0, 0 }, *viTileSize, olc::RED);

			playerDecal = new olc::Decal(playerSprite);

			pge->SetDrawTarget(nullptr);

		}

		void RenderSprite(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			pge->DrawDecal(*viCameraOffset * *viTileSize, playerDecal);

		}	

	};
}

#endif