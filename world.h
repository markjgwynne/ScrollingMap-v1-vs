
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

		void GenerateTiles()
		{
			for (int y = 0; y < viChunkTileCount->y; y++) {
				for (int x = 0; x < viChunkTileCount->x; x++) {			
					vTiles.push_back(std::make_unique<tile>(olc::vi2d(viPosition.x + x, viPosition.y + y), viTileSize));
				}
			}
		}

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

			for (auto& tile : vTiles) // access by reference to avoid copying
			{
				tile->Render(pge, viCameraOffset);
			}

		}

	};
	
    class GameWorld {
        public:

			int seed = 123;

			olc::vi2d* viChunkCount;
			olc::vi2d* viChunkTileCount;
			olc::vi2d* viTileSize;
			olc::vi2d* viRenderDistance;

			int iChunkIndex, iTileIndex, iChunkX, iChunkY;

			std::string sChunkLocation, sTileLocation, sSpriteRenderLocation, sTileAwareness;

			std::vector<std::unique_ptr<chunk>> vChunk;
			std::vector<std::unique_ptr<olc::vi2d>> vChunkIndexes;
			std::vector<int> vChunkActiveIndexes;

			std::vector<olc::Sprite*> chunkSprite;
			std::vector<olc::Decal*> chunkDecal;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize, olc::vi2d* renderDistance) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
				viRenderDistance = renderDistance;

            }

			~GameWorld() {

			}

			void GenerateWorld(int worldSeed, olc::vi2d* viStartingPosition) {


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

				// starting chunk
				int chunkX = std::floor((viStartingPosition->x / viChunkTileCount->x));
				int chunkY = std::floor((viStartingPosition->y / viChunkTileCount->y));
				int chunkIndex = chunkY * viChunkCount->y + chunkX;

				for (int y = chunkY - viRenderDistance->y; y < chunkY + viRenderDistance->y; y++) {
					for (int x = chunkX - viRenderDistance->x; x < chunkX + viRenderDistance->x; x++) {
						GenerateChunk(x, y);
					}
				}

				// assign the starting chunks as visible
				for (int i = 0; i < vChunkIndexes.size(); i++) {
					vChunkActiveIndexes.push_back(i);
				}
			}

			void GenerateChunk(int x, int y)
			{
				vChunk.push_back(std::make_unique<chunk>(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y),
					viChunkTileCount, viTileSize));
				vChunkIndexes.push_back(std::make_unique<olc::vi2d>(vChunkIndexes.size(), olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y)));
			}

			bool UpdatePlayerPosition(olc::PixelGameEngine* pge, olc::vi2d* viNextPosition, olc::vi2d* viCurrentPosition, xDirection xD, yDirection yD) {

				if (viCurrentPosition == viNextPosition) {
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
					UpdateChunks(chunkX, chunkY, viNextPosition, viCurrentPosition, xD, yD);
				}

				// save the positions for use when determining world position
				iChunkIndex = chunkIndex;
				iTileIndex = tileIndex;
				iChunkX = chunkX;
				iChunkY = chunkY;

				MovePlayer();

				return true;
				
			}

			void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				// pixel rendering
				for (int i = 0; i < vChunkActiveIndexes.size(); i++)
				{
					vChunk[vChunkActiveIndexes[i]]->Render(pge, viCameraOffset);
					// render the chunk index in the top left corner for debugging
					//pge->DrawString((chunk->viPosition + *viCameraOffset) * *viTileSize, std::to_string(index), olc::BLACK);
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

			void UpdateChunks(int chunkY, int chunkX, olc::vi2d* viNextPosition, olc::vi2d* viCurrentPosition, xDirection xD, yDirection yD) {

				//////////////////////////////////////////////
				// how on earth do i do this?
				// 
				// i need to check if the new location chunks exist
				// detect the movement first, ie y - 1 or x + 1, etc.
				// then determine if the render distance opposite to the previous movement exists

				int renderChunkY, renderChunkX;

				if (xD == xDirection::EAST) {
					renderChunkX = viNextPosition->x + 1;
				}
				else if (xD == xDirection::WEST) {
					renderChunkX = viNextPosition->x - 1;
				}

				if (yD == yDirection::SOUTH) {
					renderChunkY = viNextPosition->y + 1;
				}
				else if (yD == yDirection::NORTH) {
					renderChunkY = viNextPosition->y - 1;
				}

				/*
				// this might be covered now by the next section...where the generation of chunks and the adding to active chunk indexes is combined together
				// generate new chunks
				for (int y = renderChunkY - viRenderDistance->y; y < renderChunkY + viRenderDistance->y; y++) {
					if (std::find(vChunkIndexes.begin(), vChunkIndexes.end(), olc::vi2d(chunkX, y)) != vChunkIndexes.end())
					{
						continue;
					}
					GenerateChunk(chunkX, y);
				}
				for (int x = renderChunkX - viRenderDistance->x; x < renderChunkX + viRenderDistance->x; x++) {
					if (std::find(vChunkIndexes.begin(), vChunkIndexes.end(), olc::vi2d(x, chunkY)) != vChunkIndexes.end())
					{
						continue;
					}
					GenerateChunk(x, chunkY);
				}
				*/

				// what about the visible chunks?
				// do i delete and re-run this? 
				// or is there a way to update it, ie replace previous with new?

				// detect the movement, ie y - 1 or x + 1, etc.
				// use this to remove the opposite end of the render distance and add the new one

				// add all chunk indexes to active chunk vector
				vChunkActiveIndexes.clear();
				for (int y = renderChunkY - viRenderDistance->y; y < renderChunkY + viRenderDistance->y; y++) {
					for (int x = renderChunkX - viRenderDistance->x; x < renderChunkX + viRenderDistance->x; x++) {
						
						auto it = find(vChunkIndexes.begin(), vChunkIndexes.end(), olc::vi2d(x, y));

						int index;
						if (it != vChunkIndexes.end())
						{
							index = (int)(it - vChunkIndexes.begin());
						}
						else {
							GenerateChunk(x, y);
							index = vChunkIndexes.size();
						}
						vChunkActiveIndexes.push_back(index);
					}
				}
			}

			void MovePlayer() {
				
				for (int y = 0; y < viChunkCount->y; y++) {
					for (int x = 0; x < viChunkCount->x; x++) {

						int i = y * viChunkCount->y + x;

						vChunk[i]->bPlayerInChunk = false;
						vChunk[i]->bRenderChunk = false;

						if (x >= iChunkX - viRenderDistance->x && x <= iChunkX + viRenderDistance->x &&
							y >= iChunkY - viRenderDistance->y && y <= iChunkY + viRenderDistance->y) {
							vChunk[i]->bRenderChunk = true;
						}
					}
				}

				vChunk[iChunkIndex]->bPlayerInChunk = true;
				sChunkLocation = "Chunk index: " + std::to_string(iChunkIndex) + " of " + std::to_string(vChunk.size());

				sTileLocation = "Tile index: " + std::to_string(iTileIndex) + " of " + std::to_string(vChunk[iChunkIndex]->vTiles.size());
				sTileAwareness = "Tile Type: " + tileTypeName[vChunk[iChunkIndex]->vTiles[iTileIndex]->eTileType];
			}

    };
}
#endif
