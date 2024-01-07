
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
			for (int y = 0; y < viChunkTileCount->y; y++) {
				for (int x = 0; x < viChunkTileCount->x; x++) {			
					vTiles.push_back(std::make_unique<tile>(olc::vi2d(viPosition.x + x, viPosition.y + y), viTileSize));
				}
			}
		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			int i = 0;
			for (auto& tile : vTiles) // access by reference to avoid copying
			{
				tile->Render(pge, viCameraOffset);
				if (i < viTileSize->x) {
					//pge->DrawString((tile->viPosition + *viCameraOffset) * *viTileSize, std::to_string(i), olc::BLACK);
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

    class GameWorld {
        public:

			int seed;

			olc::vi2d* viChunkCount;
			olc::vi2d* viChunkTileCount;
			olc::vi2d* viTileSize;
			olc::vi2d* viRenderDistance;

			int iChunkIndex, iTileIndex, iChunkX, iChunkY;

			std::string sChunkLocation, sTileLocation, sSpriteRenderLocation, sTileAwareness;

			int iChunkCount;
			std::vector<std::unique_ptr<chunk>> vChunk;
			std::vector<chunkIndex> vChunkIndexes;
			std::vector<int> vChunkActiveIndexes;

			std::vector<olc::Sprite*> chunkSprite;
			std::vector<olc::Decal*> chunkDecal;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize, olc::vi2d* renderDistance) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
				viRenderDistance = renderDistance;
				iChunkCount = 0;
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

				// we want to generate the chunks surrounding the starting position.
				// refactor the GenerateChunks function to use the vfChunkPosition argument (and maybe rename it)
				// this argument should somehow determine which location to generate the chunk in.

				// Chunk positioning is based on a grid of chunktilecount * tilesize.
				// If a player is occupying a chunk and goes within X distance of a chunk edge, that chunk should be loaded ready.

				// try using X as one chunk, ie load all chunks within 1 chunk of player

				/*
				// starting chunk x and y coordinates
				int chunkX = std::floor((viStartingPosition->x / viChunkTileCount->x));
				int chunkY = std::floor((viStartingPosition->y / viChunkTileCount->y));
				
				// add all the chunks within render distance of the starting position
				for (int y = chunkY - viRenderDistance->y; y <= chunkY + viRenderDistance->y; y++) {
					for (int x = chunkX - viRenderDistance->x; x <= chunkX + viRenderDistance->x; x++) {
						int index = GenerateChunk(x, y) - 1;

						// set the chunk index (where the player is)
						if (x == chunkX && y == chunkY) {
							iChunkIndex = index;
						}
					}
				}
				*/

				olc::vi2d startingPosition = olc::vi2d(3, 3);
				olc::vi2d nextPosition = olc::vi2d(0, 0);

				/*
				* Reimplement this later
				bool collision = true;
				do {
					startingPosition.x = rand() % 10 + 1;
					startingPosition.y = rand() % 10 + 1;
					collision = UpdatePlayerPosition(pge, &nextPosition, &startingPosition);
				} while (collision == true);
				*/

				// get chunk and tile positions
				int chunkX = std::floor((startingPosition.x / viChunkTileCount->x));
				int chunkY = std::floor((startingPosition.y / viChunkTileCount->y));
				
				// does this still work? i dont think so
				int chunkIndex = chunkY * viChunkCount->y + chunkX;

				int tileX = std::fmodf(startingPosition.x, (float)viChunkTileCount->x);
				int tileY = std::fmodf(startingPosition.y, (float)viChunkTileCount->y);
				int tileIndex = (tileY * viChunkTileCount->y + tileX);

				// update the world and move player
				UpdateChunks(chunkX, chunkY);

				// sort the chunk indexes
				std::sort(vChunkIndexes.begin(), vChunkIndexes.end(), CompareByPosition());

				// save the positions for use when determining world position
				iChunkIndex = chunkIndex;
				iTileIndex = tileIndex;
				iChunkX = chunkX;
				iChunkY = chunkY;

				MovePlayerMapPosition();

				return startingPosition;
			}

			int GenerateChunk(int x, int y)
			{
				vChunk.push_back(std::make_unique<chunk>(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y),
					viChunkTileCount, viTileSize));
				vChunkIndexes.push_back(chunkIndex(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y), iChunkCount));
				iChunkCount += 1;
				return iChunkCount;

			}

			bool UpdatePlayerPosition(olc::PixelGameEngine* pge, olc::vi2d* viCurrentPosition, olc::vi2d* viNextPosition) {

				/*
				* split out all of the actions here to perform one specific task only
				* 
				* use the chunkX and chunkY to create an olc::vi2d object and find the cChunkIndexes object that matches.
				*	(the above replaces: int chunkIndex = chunkY * viChunkCount->y + chunkX;)
				*/

				if (viCurrentPosition->x == viNextPosition->x && viCurrentPosition->y == viNextPosition->y) {
					// no player movement, just update the world
					return false;
				}

				// the player did move to a new tile

				// get chunk and tile positions
				int chunkX = std::floor((viNextPosition->x / viChunkTileCount->x));
				int chunkY = std::floor((viNextPosition->y / viChunkTileCount->y));
				int chunkIndex = chunkY * viChunkCount->y + chunkX;
				
				int tileX = std::fmodf(viNextPosition->x, (float)viChunkTileCount->x);
				int tileY = std::fmodf(viNextPosition->y, (float)viChunkTileCount->y);
				int tileIndex = (tileY * viChunkTileCount->y + tileX);

				// determine if collision in tile
				if (IsCollision(chunkIndex, tileIndex) == true) {
					// collision
					// no player movement
					// update the world and run collision action
					return false;
				}
				
				// no collision
				// update the world and move player
				if (iChunkIndex != chunkIndex) {
					UpdateChunks(chunkX, chunkY);
				}

				// sort the chunk indexes
				std::sort(vChunkIndexes.begin(), vChunkIndexes.end(), CompareByPosition());

				// save the positions for use when determining world position
				iChunkIndex = chunkIndex;
				iTileIndex = tileIndex;
				iChunkX = chunkX;
				iChunkY = chunkY;

				MovePlayerMapPosition();

				return true;
				
			}

			void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				// pixel rendering
				for (int i = 0; i < vChunkActiveIndexes.size(); i++)
				{
					vChunk[vChunkIndexes[vChunkActiveIndexes[i]].index]->Render(pge, viCameraOffset);
					// render the chunk index in the top left corner for debugging
					pge->DrawString((vChunk[vChunkActiveIndexes[i]]->viPosition + *viCameraOffset) * *viTileSize, std::to_string(i), olc::BLACK);
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
					// return true;
					return true;
				}
				else {
					// no collision, continue rendering normal movement 
					// return false
					
					return false;
				}

			}
			
			void UpdateChunks(int positionChunkX, int positionChunkY) {

				//////////////////////////////////////////////
								
				// what about the visible chunks?
				// do i delete and re-run this? 
				// or is there a way to update it, ie replace previous with new?

				// add all chunk indexes to active chunk vector
				
				vChunkActiveIndexes.clear();
				for (int y = positionChunkY - viRenderDistance->y; y <= positionChunkY + viRenderDistance->y; y++) {
					for (int x = positionChunkX - viRenderDistance->x; x <= positionChunkX + viRenderDistance->x; x++) {
						
						auto it = find(vChunkIndexes.begin(), vChunkIndexes.end(), olc::vi2d(x, y));

						int index;
						if (it != vChunkIndexes.end())
						{
							index = (int)(it - vChunkIndexes.begin());
						}
						else {
							index = GenerateChunk(x, y) - 1;
						}
						vChunkActiveIndexes.push_back(index);
												
					}
				}

			}

			void MovePlayerMapPosition() {

				/*
				for (int i = 0; i < vChunkActiveIndexes.size(); i++) {
					vChunk[vChunkIndexes[i].index]->bPlayerInChunk = false;
				}
				*/

				//vChunk[vChunkIndexes[iChunkIndex].index]->bPlayerInChunk = true;
				sChunkLocation = "Chunk index: " + std::to_string(iChunkIndex) + " of " + std::to_string(vChunk.size());

				sTileLocation = "Tile index: " + std::to_string(iTileIndex) + " of " + std::to_string(vChunk[iChunkIndex]->vTiles.size());
				sTileAwareness = "Tile Type: " + tileTypeName[vChunk[iChunkIndex]->vTiles[iTileIndex]->eTileType];

			}
						
    };
}
#endif
