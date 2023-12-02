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

		Character(olc::vf2d *position, olc::vi2d *tileSize) {

			vfPosition = position;
			viTileSize = tileSize;

		}
		~Character() {
			delete vfPosition;
			delete viTileSize;
		}

		void Update(olc::PixelGameEngine* pge) {

			if (pge->GetKey(olc::Key::W).bPressed) vfPosition->y -= 1;
			if (pge->GetKey(olc::Key::S).bPressed) vfPosition->y += 1;
			if (pge->GetKey(olc::Key::A).bPressed) vfPosition->x -= 1;
			if (pge->GetKey(olc::Key::D).bPressed) vfPosition->x += 1;

			;

		}

		void Render(olc::PixelGameEngine* pge) {

			pge->FillRect(*vfPosition * *viTileSize, *viTileSize, olc::RED);

		}

	};
}

#endif