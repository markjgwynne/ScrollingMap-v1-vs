
using namespace std;

#include <iostream>
#include <random>
#include <map>
#include <math.h>
#include <algorithm>
#include <vector>

#include "sharedUtilities.h"
#include "olcPixelGameEngine.h"

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

namespace ScrollingMap
{

	class tile 
	{
	public:
		olc::vi2d viPosition;
		olc::vi2d *viTileSize;
		tiletype eTileType;

		tile() {};

		tile(olc::vi2d position, olc::vi2d* tileSize) {
			
			SetPosition(position, tileSize);
		}

		~tile() {
			
		}

		void SetPosition(olc::vi2d position, olc::vi2d* tileSize) {
			viPosition = position;
			viTileSize = tileSize;
			eTileType = Grass;

			AddLayerTrees();
		}

		void AddLayerTrees() {

			if (rand() % 6 == 0) eTileType = Tree;

		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viOffset) {

			pge->FillRect((viPosition + *viOffset) * *viTileSize, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);

		}

	};

	class chunk
	{
	public:
		olc::vf2d viPosition;
		olc::vi2d *viChunkTileCount;
		olc::vi2d *viTileSize;
		bool bPlayerInChunk;
		bool bRenderChunk;

		std::vector<std::unique_ptr<tile>> vTiles;
		
		chunk(olc::vi2d position, olc::vi2d* tileCount, olc::vi2d* tileSize) {
			
			viPosition = position;
			viChunkTileCount = tileCount;
			viTileSize = tileSize;

			bPlayerInChunk = false;
			bRenderChunk = false;
			
			GenerateTiles();
		}

		~chunk() {
			
		}

		bool operator< (const chunk& other) const {
			return viPosition.x < other.viPosition.x && viPosition.y < other.viPosition.y;
		}

		void GenerateTiles()
		{
			if (viPosition.x < 0) {
				bool testing = true;
			}
			// do something here to detect if the y is less than zero.
			// do i need to do the same for x?
			// ie, it counts from x = -16 to 0 which is correct, but starts at y = -16, then works up, 
			//    which means the lower indexes are at the bottom of the chunk!
			// ##################################################################################################
			
			/*
			* My entire understanding of chunk rendering changed. The above was to troubleshoot problems with identification of the tileindex
			* I was using my chunk starting location when in negative (-16, -16) as the starting point, then counting up from there.
			* While this seems fine, the order gets messed up. The order / indexes of tiles needs to go from point (0, 0) outwards. It needs to
			* count up in the positive directions and down in the negative direction.
			* This takes some work, as can be seen below.
			* 
			* Now this is done, the method of determining the tileIndex needs work!
			*/

			// the minus one against the viChunkTileCount when initialising is to ensure only one chunk takes the 0, 0 x and y.
			
			for (int y = (viPosition.y < 0) ? viChunkTileCount->y -1 : 0; (viPosition.y < 0) ? y >= 0 : y < viChunkTileCount->y; y+=(viPosition.y < 0) ? -1 : 1 ) {
				for (int x = (viPosition.x < 0) ? viChunkTileCount->x-1 : 0; (viPosition.x < 0) ? x >= 0 : x < viChunkTileCount->x; x+=(viPosition.x < 0) ? -1 : 1 ) {
					vTiles.push_back(std::make_unique<tile>(olc::vi2d(viPosition.x + x, viPosition.y + y), viTileSize));
				}
			}
		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			int i = 0;
			for (auto& tile : vTiles) // access by reference to avoid copying
			{
				tile->Render(pge, viCameraOffset);
				if (i < 10) {
					pge->DrawString((tile->viPosition + *viCameraOffset) * *viTileSize, std::to_string(i), olc::BLACK);
				}
				i += 1;
			}

		}

	};

	struct chunkIndex {
		olc::vi2d position;
		int index;
		chunkIndex(olc::vi2d vPosition, int iIndex) {
			position = vPosition;
			index = iIndex;
		}
		
		bool operator> (const chunkIndex&) const;
		bool operator>=(const chunkIndex&) const;
		bool operator==(const chunkIndex&) const;
		bool operator<=(const chunkIndex&) const;
		bool operator< (const chunkIndex&) const;

		bool operator> (const olc::vi2d&) const;
		bool operator>=(const olc::vi2d&) const;
		bool operator==(const olc::vi2d&) const;
		bool operator<=(const olc::vi2d&) const;
		bool operator< (const olc::vi2d&) const;
		
	};
	bool chunkIndex::operator> (const chunkIndex& c) const { return c.position.x > position.x && c.position.y > position.y; }
	bool chunkIndex::operator>=(const chunkIndex& c) const { return c.position.x >= position.x && c.position.y >= position.y; }
	bool chunkIndex::operator==(const chunkIndex& c) const { return c.position.x == position.x && c.position.y == position.y; }
	bool chunkIndex::operator<=(const chunkIndex& c) const { return c.position.x <= position.x && c.position.y <= position.y; }
	bool chunkIndex::operator< (const chunkIndex& c) const { return c.position.x < position.x && c.position.y < position.y; }
	
	bool chunkIndex::operator> (const olc::vi2d& v) const { return v.x > position.x && v.y > position.y; }
	bool chunkIndex::operator>=(const olc::vi2d& v) const { return v.x >= position.x && v.y >= position.y; }
	bool chunkIndex::operator==(const olc::vi2d& v) const { return v.x == position.x && v.y == position.y; }
	bool chunkIndex::operator<=(const olc::vi2d& v) const { return v.x <= position.x && v.y <= position.y; }
	bool chunkIndex::operator< (const olc::vi2d& v) const { return v.x < position.x && v.y < position.y; }
	
	// this set of comparison operators allows the comparison of a position to a chunkIndex
	bool operator> (const olc::vi2d& v, const chunkIndex& c) { return v.x > c.position.x && v.y > c.position.y; }
	bool operator>=(const olc::vi2d& v, const chunkIndex& c) { return v.x >= c.position.x && v.y >= c.position.y; }
	bool operator==(const olc::vi2d& v, const chunkIndex& c) { return v.x == c.position.x && v.y == c.position.y; }
	bool operator<=(const olc::vi2d& v, const chunkIndex& c) { return v.x <= c.position.x && v.y <= c.position.y; }
	bool operator< (const olc::vi2d& v, const chunkIndex& c) { return v.x < c.position.x && v.y < c.position.y; }


	// A functor to compare two persons by their age
	struct CompareByPosition {
		bool operator() (const chunkIndex& p1, const chunkIndex& p2) {
			return p1.position.x < p2.position.x && p1.position.y < p2.position.y;
		}
	};

	struct worldPosition {
		olc::vi2d viChunkTileCount;
		olc::vi2d viPosition;
		int iChunkX;
		int iChunkY;
		int iChunkIndex;
		olc::vi2d viChunkPosition;
		int iTileX;
		int iTileY;
		int iTileIndex;		
		std::vector<chunkIndex>* vChunkIndexes;

		worldPosition()
		{
		};
		
		worldPosition(olc::vi2d* chunkTileCount, std::vector<chunkIndex>* chunkIndexes) {
			viChunkTileCount = *chunkTileCount;
			vChunkIndexes = chunkIndexes;
		}
		int getChunkIndexes(int position, float tileCount) {
			int index = std::floor(position / tileCount);
			return index;
		}
		int getTileIndex(int pX, int pY, float tileCount) {

			if (pX < 0) pX = std::abs(pX) - 1;
			if (pY < 0) pY = std::abs(pY) - 1;
			int index = pY * tileCount + pX;
			return index;
		}
		bool setWorldPosition(olc::vi2d* position) {
			// THIS IS NOT WORKING ##########################################################################################
			// the chunk index appears to be not working now that the tile generation is working.
			// check the negative coordinate logic for chunk determination
			// then check the tileIndex determination

			viPosition = *position;
			
			// get chunk position
			iChunkX = getChunkIndexes(viPosition.x, (float)viChunkTileCount.x);
			iChunkY = getChunkIndexes(viPosition.y, (float)viChunkTileCount.y);
			
			viChunkPosition = olc::vi2d(iChunkX, iChunkY);

			iTileIndex = getTileIndex(position->x, position->y, (float)viChunkTileCount.x);
			
			auto item = find(vChunkIndexes->begin(), vChunkIndexes->end(), viChunkPosition);

			if (item != vChunkIndexes->end())
			{
				iChunkIndex = item->index;// (int)(item - vChunkIndexes->begin());
				return true;
			}
			else {
				iChunkIndex = -1;
				return false;
			}
		}
	};

    class GameWorld {
        public:

			int seed;

			olc::vi2d* viChunkCount;
			olc::vi2d* viChunkTileCount;
			olc::vi2d* viTileSize;
			olc::vi2d* viRenderDistance;
			
			std::string sChunkLocation, sTileLocation, sSpriteRenderLocation, sTileAwareness;

			int iChunkCount;
			std::vector<std::unique_ptr<chunk>> vChunk;
			std::vector<chunkIndex> vChunkIndexes;
			std::vector<int> vChunkActiveIndexes;

			std::vector<olc::Sprite*> chunkSprite;
			std::vector<olc::Decal*> chunkDecal;

			int iChunkIndex, iTileIndex, iChunkX, iChunkY;
			worldPosition positionPrevious;
			worldPosition positionCurrent;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize, olc::vi2d* renderDistance) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
				viRenderDistance = renderDistance;
				iChunkCount = 0;

				positionPrevious = worldPosition(viChunkTileCount, &vChunkIndexes);
				positionCurrent = worldPosition(viChunkTileCount, &vChunkIndexes);
            }

			~GameWorld() {

			}

			olc::vi2d GenerateWorld(olc::PixelGameEngine* pge, int worldSeed) {

				//doesnt currently do anything.
				/*
				seed = worldSeed;

				// Create a random number generator and seed it with a fixed value
				std::mt19937 generator(seed);

				// Create a distribution range
				std::uniform_int_distribution<int> distribution(1, 100);

				// Generate and print 5 random numbers
				for (int i = 0; i < 5; ++i) {
					int random_number = distribution(generator);
				}
				*/
				
				olc::vi2d startingPosition = olc::vi2d(3, 3);
				/*
				* Reimplement this later to upgrade the starting position above to random
				* also update it to allow random in a negative and positive direction
				startingPosition.x = rand() % 10 + 1;
				startingPosition.y = rand() % 10 + 1;
				bool collision = true;
				do {
					startingPosition.x += 1; 
					startingPosition.y += 1;
					collision = UpdatePlayerPosition(pge, &nextPosition, &startingPosition);
				} while (collision == true);
				*/

				positionCurrent.setWorldPosition(&startingPosition);
				positionPrevious = positionCurrent;

				// update the world and move player
				LoadChunks(positionCurrent.iChunkX, positionCurrent.iChunkY);

				positionCurrent.setWorldPosition(&startingPosition); // recall the method to set the chunk index

				/*
				* // this is useful elsewhere maybe?
				if (positionCurrent.setWorldPosition(&startingPosition) == false) {
					// indicates the chunk doesnt exist
					positionCurrent.iChunkIndex = GenerateChunk(positionCurrent.iChunkX, positionCurrent.iChunkY) - 1;
				}
				*/

				// sort the chunk indexes
				std::sort(vChunkIndexes.begin(), vChunkIndexes.end(), CompareByPosition());

				MovePlayerMapPosition();

				return startingPosition;
			}

			bool UpdatePlayerPosition(olc::PixelGameEngine* pge, olc::vi2d* viCurrentPosition, olc::vi2d* viNextPosition) {

				// Chunk positioning is based on a grid of chunktilecount * tilesize.
				// If a player is occupying a chunk and goes within X distance of a chunk edge, that chunk should be loaded ready.

				// try using X as one chunk, ie load all chunks within 1 chunk of player

				/*
				* split out all of the actions here to perform one specific task only
				* 
				* use the chunkX and chunkY to create an olc::vi2d object and find the cChunkIndexes object that matches.
				*	(the above replaces: int chunkIndex = chunkY * viChunkCount->y + chunkX;)
				*/

				if (viCurrentPosition->x == viNextPosition->x && viCurrentPosition->y == viNextPosition->y) {
					// no player movement, therefore make the previous world position match the current world position
					positionPrevious = positionCurrent;

					// update the world

					return false;
				}

				// the player did move to a new tile
				// back up the world position
				positionPrevious = positionCurrent;

				// set the new world position
				positionCurrent.setWorldPosition(viNextPosition);

				
				// determine if collision in tile
				if (IsCollision(positionCurrent.iChunkIndex, positionCurrent.iTileIndex) == true) {
					// collision
					// no player movement, therefore make the current world position match the previous world position
					// the previous world position can stay as is, because a movement happened but failed.
					positionCurrent = positionPrevious;

					// update the world and run collision action

					return false;
				}
				
				
				// no collision
				// update the world and move player
				if (positionCurrent.iChunkIndex != positionPrevious.iChunkIndex) {
					LoadChunks(positionCurrent.iChunkX, positionCurrent.iChunkY);
				}

				// sort the chunk indexes
				std::sort(vChunkIndexes.begin(), vChunkIndexes.end(), CompareByPosition());

				MovePlayerMapPosition();

				return true;
				
			}

			void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				// pixel rendering
				for (int i = 0; i < vChunkActiveIndexes.size(); i++)
				{
					vChunk[vChunkIndexes[vChunkActiveIndexes[i]].index]->Render(pge, viCameraOffset);
					// render the chunk index in the top left corner for debugging
					pge->DrawString((vChunk[vChunkIndexes[vChunkActiveIndexes[i]].index]->viPosition + *viCameraOffset) * *viTileSize, std::to_string(vChunkIndexes[vChunkActiveIndexes[i]].index), olc::BLACK);
				}

			}
			
			void GenerateSprite(olc::PixelGameEngine* pge) {

				olc::Sprite* sprBackground = new olc::Sprite(viChunkCount->x * viChunkTileCount->x * viTileSize->x, viChunkCount->x * viChunkTileCount->x * viTileSize->x);

				pge->SetDrawTarget(sprBackground);

				olc::vi2d offset = { 0, 0 };

				for (auto& chunk : vChunk) // access by reference to avoid copying
				{
					chunk->Render(pge, &offset);
				}

				chunkSprite.push_back(sprBackground);

				olc::Decal* decBackground = new olc::Decal(sprBackground);

				chunkDecal.push_back(decBackground);

				// Don't foregt to set the draw target back to being the main screen (been there... wasted 1.5 hours :| )
				pge->SetDrawTarget(nullptr);
			}

			void RenderSprite(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				// sprite/decal rendering

				pge->DrawPartialDecal({ 0.0f, 0.0f }, chunkDecal[0], *viCameraOffset * *viTileSize, { (float)pge->ScreenWidth(), (float)pge->ScreenHeight() });

				sSpriteRenderLocation = "Sprite render location, x:" + std::to_string(viCameraOffset->x * viTileSize->x) + " | y: " + std::to_string(viCameraOffset->y * viTileSize->y);

			}

		private:
			
			bool IsCollision(int chunkIndex, int tileIndex) {
				//x * chunkCountWidth + y;

				if (vChunk[chunkIndex]->vTiles[tileIndex]->eTileType == Tree) {
					// collision. update nothing
					return false;
				}
				else {
					// no collision, continue rendering normal movement 					
					return false;
				}

			}

			int GenerateChunk(int x, int y)
			{
				vChunk.push_back(std::make_unique<chunk>(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y),
					viChunkTileCount, viTileSize));
				//vChunkIndexes.push_back(chunkIndex(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y), iChunkCount));
				vChunkIndexes.push_back(chunkIndex(olc::vi2d(x, y), iChunkCount));
				iChunkCount += 1;
				return iChunkCount;

			}
			
			void LoadChunks(int positionChunkX, int positionChunkY) {

				//////////////////////////////////////////////
				// add all chunk indexes to active chunk vector
				
				vChunkActiveIndexes.clear();
				for (int y = positionChunkY - viRenderDistance->y; y <= positionChunkY + viRenderDistance->y; y++) {
					for (int x = positionChunkX - viRenderDistance->x; x <= positionChunkX + viRenderDistance->x; x++) {
						
						auto it = find(vChunkIndexes.begin(), vChunkIndexes.end(), olc::vi2d(x, y));

						int index;
						if (it != vChunkIndexes.end())
						{
							index = it->index;
						}
						else {
							index = GenerateChunk(x, y) - 1;
						}
						vChunkActiveIndexes.push_back(index);
												
					}
				}

			}

			void MovePlayerMapPosition() {

				sChunkLocation = "Chunk index: " + std::to_string(positionCurrent.iChunkIndex) + " of " + std::to_string(vChunk.size());
				sTileLocation = "Tile index: " + std::to_string(positionCurrent.iTileIndex) + " of " + std::to_string(vChunk[positionCurrent.iChunkIndex]->vTiles.size());
				sTileAwareness = "Tile Type: " + tileTypeName[vChunk[positionCurrent.iChunkIndex]->vTiles[positionCurrent.iTileIndex]->eTileType];

			}
						
    };
}
#endif
