
using namespace std;

#include <iostream>
#include <random>
#include <map>
#include <math.h>

#include "olcPixelGameEngine.h"

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

namespace ScrollingMap
{

	enum tiletype
	{
		Grass,
		Tree,
		Sand,
		Path
	};

	std::map<tiletype, olc::Pixel> tileMap;

	class tile 
	{
	public:
		olc::vi2d viPosition;
		olc::vi2d *viTileSize;
		tiletype eTileType;

		tile(olc::vi2d position, olc::vi2d* tileSize) {
			viPosition = position;
			viTileSize = tileSize;
			eTileType = Grass;

			AddLayerTrees();
		};

		~tile() {
			//delete viTileSize;
		};

		void AddLayerTrees() {

			if (rand() % 6 == 0) eTileType = Tree;

		};

		void Render(olc::PixelGameEngine* pge, olc::vi2d* viOffset) {

			// original
			pge->FillRect((viPosition + *viOffset) * *viTileSize, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);

			//vfCameraOffset -= vfPlayerPos;

			//olc::vi2d vfWorldSpace = vfPosition  - (*vfPlayerPos - *vfCameraOffset);

			//pge->FillRect(vfWorldSpace * *viTileSize, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);

			//pge->FillRect(vfPosition + *vfOffset, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);

		};

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
			//delete viChunkTileCount;
			//delete viTileSize;
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

			int iChunkIndex, iTileIndex;

			std::string sChunkLocation, sTileLocation, sTileAwareness;

			std::vector<std::unique_ptr<chunk>> vChunk;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;

				tileMap[Grass] = olc::GREEN;
				tileMap[Tree] = olc::DARK_GREEN;
				tileMap[Sand] = olc::DARK_YELLOW;
				tileMap[Path] = olc::GREY;

            }

			~GameWorld() {
				//delete viChunkCount;
				//delete viChunkTileCount;
				//delete viTileSize;
			}

			void GenerateWorld(int worldSeed) {

				seed = worldSeed;

				// Create a random number generator and seed it with a fixed value
				std::mt19937 generator(seed);

				// Create a distribution range
				std::uniform_int_distribution<int> distribution(1, 100);

				// Generate and print 5 random numbers
				for (int i = 0; i < 5; ++i) {
					int random_number = distribution(generator);
				}

			}
			
			void GenerateChunks(olc::vi2d* vfChunkPosition)
			{
				for (int y = 0; y < viChunkCount->y; y++) {
					for (int x = 0; x < viChunkCount->x; x++) {				
						vChunk.push_back(std::make_unique<chunk>(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y),
							viChunkTileCount, viTileSize));
					}
				}

			}

			bool isCollision(olc::vi2d* viNextPosition) {

				if(vChunk[iChunkIndex]->vTiles[iTileIndex]->eTileType == Tree) {
					return true;
				} else {
					return false;
				};

			}

			void Update(olc::PixelGameEngine* pge, olc::vi2d* viPlayerPosition, olc::vi2d* viRenderDistance) {
				//x * chunkCountWidth + y;

				int chunkX = std::floor((viPlayerPosition->x / viChunkTileCount->x));
				int chunkY = std::floor((viPlayerPosition->y / viChunkTileCount->y));
				
				for (int y = 0; y < viChunkCount->y; y++) {
					for (int x = 0; x < viChunkCount->x; x++) {

						vChunk[x * viChunkCount->x + y]->bPlayerInChunk = false;
						
						if (x >= chunkX - viRenderDistance->x && x <= chunkX + viRenderDistance->x &&
							y >= chunkY - viRenderDistance->y && y <= chunkY + viRenderDistance->y) {
							vChunk[x * viChunkCount->x + y]->bRenderChunk = true;
						}
						else {
							vChunk[x * viChunkCount->x + y]->bRenderChunk = false;
						}
					}
				}
							
				iChunkIndex = chunkX * viChunkCount->x + chunkY;
				vChunk[iChunkIndex]->bPlayerInChunk = true;
				sChunkLocation = "Chunk index: " + std::to_string(iChunkIndex) + " of " + std::to_string(vChunk.size());

				int tileX = std::fmodf(viPlayerPosition->x, (float)viChunkTileCount->x);
				int tileY = std::fmodf(viPlayerPosition->y, (float)viChunkTileCount->y);

				iTileIndex = (tileY * viChunkTileCount->y - tileX) - 1;
				sTileLocation = "Tile index: " + std::to_string(iTileIndex) + " of " + std::to_string(vChunk[iChunkIndex]->vTiles.size());
				//sTileAwareness = "Tile Type: " + std::to_string(vChunk[iChunkIndex]->vTiles[iTileIndex]->eTileType);

			}

			void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				for (auto& chunk : vChunk) // access by reference to avoid copying
				{
					if (chunk->bRenderChunk) {
					chunk->Render(pge, viCameraOffset);
					}
				}
			
			}

    };
}
#endif
