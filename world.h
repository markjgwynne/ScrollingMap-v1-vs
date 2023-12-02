
using namespace std;

#include <iostream>
#include <random>

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

	class tile 
	{
	public:
		olc::vf2d vfPosition;
		olc::vi2d *viTileSize;
		tiletype eTileType;

		tile(olc::vf2d position, olc::vi2d *tileSize) {
			vfPosition = position;
			viTileSize = tileSize;
		}

		~tile() {
			delete viTileSize;
		}

		void Render(olc::PixelGameEngine* pge) {
			pge->FillRect(vfPosition, olc::vi2d(viTileSize->x, viTileSize->y), olc::GREEN);
		}

	};

	class chunk
	{
	public:
		olc::vf2d vfPosition;
		olc::vi2d *viChunkTileCount;
		olc::vi2d *viTileSize;

		std::vector<std::unique_ptr<tile>> vTiles;

		chunk(olc::vf2d position, olc::vi2d *tileCount, olc::vi2d *tileSize) {
			
			vfPosition = position;
			viChunkTileCount = tileCount;
			viTileSize = tileSize;

			GenerateTiles();
		}

		~chunk() {
			delete viChunkTileCount;
			delete viTileSize;
		}

		void GenerateTiles()
		{
			for (int x = 0; x < viChunkTileCount->x; x++) {
				for (int y = 0; y < viChunkTileCount->y; y++) {
					vTiles.push_back(std::make_unique<tile>(olc::vf2d(vfPosition.x + (x * viTileSize->x), vfPosition.y + (y * viTileSize->y)), viTileSize));
				}
			}
		}

		void Render(olc::PixelGameEngine* pge) {

			for (auto& tile : vTiles) // access by reference to avoid copying
			{
				tile->Render(pge);
			}

		}

	};
	
    class GameWorld {
        public:

			int seed = 123;

			olc::vi2d *viChunkCount;
			olc::vi2d *viChunkTileCount;
			olc::vi2d *viTileSize;

			std::vector<std::unique_ptr<chunk>> vChunk;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
            };

            ~GameWorld() {
				delete viChunkCount;
				delete viChunkTileCount;
				delete viTileSize;
            };

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
						
			void GenerateChunks(olc::vf2d vfPlayerPosition)
			{
				for (int x = 0; x < viChunkCount->x; x++) {
					for (int y = 0; y < viChunkCount->y; y++) {
						vChunk.push_back(std::make_unique<chunk>(olc::vf2d(x * (viChunkTileCount->x * viTileSize->x), y * (viChunkTileCount->y * viTileSize->y)),
							viChunkTileCount, viTileSize));
					}
				}

			}

			void Render(olc::PixelGameEngine* pge) {

				for (auto& chunk : vChunk) // access by reference to avoid copying
				{
					chunk->Render(pge);
				}

			}

    };
}
#endif
