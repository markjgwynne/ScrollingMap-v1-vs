
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
		olc::vf2d vfPosition;
		olc::vi2d *viTileSize;
		tiletype eTileType;

		tile(olc::vf2d position, olc::vi2d* tileSize) {
			vfPosition = position;
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

		void Render(olc::PixelGameEngine* pge, olc::vf2d* vfCameraOffset, olc::vf2d* vfPlayerPos) {

			//pge->FillRect(vfPosition, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);


			//vfCameraOffset -= vfPlayerPos;

			olc::vi2d vfWorldSpace = *vfCameraOffset - *vfPlayerPos;

			pge->FillRect(vfWorldSpace * *viTileSize, olc::vi2d(viTileSize->x, viTileSize->y), tileMap[eTileType]);
			
		};

	};

	class chunk
	{
	public:
		olc::vf2d vfPosition;
		olc::vi2d *viChunkTileCount;
		olc::vi2d *viTileSize;
		bool bPlayerInChunk;

		std::vector<std::unique_ptr<tile>> vTiles;

		chunk(olc::vf2d position, olc::vi2d* tileCount, olc::vi2d* tileSize) {
			
			vfPosition = position;
			viChunkTileCount = tileCount;
			viTileSize = tileSize;

			bPlayerInChunk = false;

			GenerateTiles();
		}

		~chunk() {
			//delete viChunkTileCount;
			//delete viTileSize;
		}

		void GenerateTiles()
		{
			for (int x = 0; x < viChunkTileCount->x; x++) {
				for (int y = 0; y < viChunkTileCount->y; y++) {
					vTiles.push_back(std::make_unique<tile>(olc::vf2d(vfPosition.x + (x * viTileSize->x), vfPosition.y + (y * viTileSize->y)), viTileSize));
				}
			}
		}

		bool PlayerInChunk(olc::vf2d* vfPlayerPosition) {

			if (vfPlayerPosition->x * viTileSize->x >= vfPosition.x && vfPlayerPosition->x * viTileSize->x < vfPosition.x + (viChunkTileCount->x * viTileSize->x) &&
				vfPlayerPosition->y * viTileSize->y >= vfPosition.y && vfPlayerPosition->y * viTileSize->y < vfPosition.y + (viChunkTileCount->y * viTileSize->y) ) {
				bPlayerInChunk = true;
			} else {
				bPlayerInChunk = false;
			}
			return bPlayerInChunk;
		}

		void Render(olc::PixelGameEngine* pge, olc::vf2d* vfCameraOffset, olc::vf2d* vfPLayerPos) {

			for (auto& tile : vTiles) // access by reference to avoid copying
			{
				tile->Render(pge, vfCameraOffset, vfPLayerPos);
			}

		}

	};
	
    class GameWorld {
        public:

			int seed = 123;

			olc::vi2d* viChunkCount;
			olc::vi2d* viChunkTileCount;
			olc::vi2d* viTileSize;

			int iChunkIndex;

			std::string sChunkLocation;

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
			
			void GenerateChunks(olc::vf2d* vfChunkPosition)
			{
				for (int x = 0; x < viChunkCount->x; x++) {
					for (int y = 0; y < viChunkCount->y; y++) {
						vChunk.push_back(std::make_unique<chunk>(olc::vf2d(x * (viChunkTileCount->x * viTileSize->x), y * (viChunkTileCount->y * viTileSize->y)),
							viChunkTileCount, viTileSize));
					}
				}

			}

			void Update(olc::PixelGameEngine* pge, olc::vf2d* vfPlayerPosition) {
				//x * chunkCountWidth + y;

				int posX = std::floor((vfPlayerPosition->x / viChunkTileCount->x));
				int posY = std::floor((vfPlayerPosition->y / viChunkTileCount->y));

				for (int x = 0; x < viChunkCount->x; x++) {
					for (int y = 0; y < viChunkCount->y; y++) {
						
						if (x >= posX - 1 && x <= posX + 1 &&
							y >= posY - 1 && y <= posY + 1) {
							vChunk[x * viChunkCount->x + y]->bPlayerInChunk = true;
						}
						else {
							vChunk[x * viChunkCount->x + y]->bPlayerInChunk = false;
						}
					}
				}
							
				//iChunkIndex = posX * viChunkCount->x + posY;

				//vChunk[iChunkIndex]->bPlayerInChunk = true;
				
				sChunkLocation = "Chunk index: " + std::to_string(iChunkIndex) + " of " + std::to_string(vChunk.size());

			}

			void Render(olc::PixelGameEngine* pge, olc::vf2d* vfCameraOffset, olc::vf2d* vfPLayerPos) {

				for (auto& chunk : vChunk) // access by reference to avoid copying
				{
					if (chunk->bPlayerInChunk) {
						chunk->Render(pge, vfCameraOffset, vfPLayerPos);
					}
				}
			
			}

    };
}
#endif
