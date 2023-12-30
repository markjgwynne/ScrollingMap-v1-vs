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

			/*
			if (pge->GetKey(olc::Key::W).bHeld) viNextPosition.y -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::S).bHeld) viNextPosition.y += iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::A).bHeld) viNextPosition.x -= iMovementSpeed * pge->GetElapsedTime();
			if (pge->GetKey(olc::Key::D).bHeld) viNextPosition.x += iMovementSpeed * pge->GetElapsedTime();
			*/

			/*
			if (viNextPosition.x > viPosition->x) {
				// EAST, positive x axis movement
				xAxisDirection = xDirection::EAST;
			} else if (viNextPosition.x > viPosition->x) {
				// WEST, negative x axis movement
				xAxisDirection = xDirection::WEST;
			} else {
				xAxisDirection = xDirection::NONE_X;
			}

			if (viNextPosition.y > viPosition->y) {
				// SOUTH, positive x axis movement
				yAxisDirection = yDirection::SOUTH;
			} else if (viNextPosition.y > viPosition->y) {
				// NORTH, negative x axis movement
				yAxisDirection = yDirection::NORTH;
			} else {
				yAxisDirection = yDirection::NONE_Y;
			}
			*/

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