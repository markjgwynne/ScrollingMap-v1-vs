
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

	std::map<tiletype, std::string> tileTypeName;
	
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
			olc::vi2d* viRenderDistance;

			int iChunkIndex, iTileIndex, iChunkX, iChunkY;

			std::string sChunkLocation, sTileLocation, sSpriteRenderLocation, sTileAwareness;

			std::vector<std::unique_ptr<chunk>> vChunk;

			std::vector<olc::Sprite*> chunkSprite;
			std::vector<olc::Decal*> chunkDecal;

			GameWorld(olc::vi2d* chunkCount, olc::vi2d* tileCount, olc::vi2d* tileSize, olc::vi2d* renderDistance) {
				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
				viRenderDistance = renderDistance;

				tileMap[Grass] = olc::GREEN;
				tileMap[Tree] = olc::DARK_GREEN;
				tileMap[Sand] = olc::DARK_YELLOW;
				tileMap[Path] = olc::GREY;

				tileTypeName[Grass] = "Grass";
				tileTypeName[Tree] = "Tree";
				tileTypeName[Sand] = "Sand";
				tileTypeName[Path] = "Path";

            }

			~GameWorld() {
				//delete viChunkCount;
				//delete viChunkTileCount;
				//delete viTileSize;
			}

			void GenerateWorld(int worldSeed) {

				//doesnt currently do anything.

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
			
			void GenerateChunks(olc::PixelGameEngine* pge, olc::vi2d* vfChunkPosition)
			{
				for (int y = 0; y < viChunkCount->y; y++) {
					for (int x = 0; x < viChunkCount->x; x++) {				
						vChunk.push_back(std::make_unique<chunk>(olc::vi2d(x * viChunkTileCount->x, y * viChunkTileCount->y),
							viChunkTileCount, viTileSize));
					}
				}

			}

			bool UpdatePlayerPosition(olc::PixelGameEngine* pge, olc::vi2d* viNextPosition) {

				if (IsCollision(pge, viNextPosition) == false) {
					MovePlayer();
					return true;
				};

				return false;

			}

			void Render(olc::PixelGameEngine* pge, olc::vi2d* viCameraOffset) {

				// pixel rendering
				int index = 0;
				for (auto& chunk : vChunk) // access by reference to avoid copying
				{
					if (chunk->bRenderChunk) {
					chunk->Render(pge, viCameraOffset);
					// render the chunk index in the top left corner for debugging
					//pge->DrawString((chunk->viPosition + *viCameraOffset) * *viTileSize, std::to_string(index), olc::BLACK);
					}
					index += 1;
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

			bool IsCollision(olc::PixelGameEngine* pge, olc::vi2d* viNextPosition) {
				//x * chunkCountWidth + y;

				int chunkX = std::floor((viNextPosition->x / viChunkTileCount->x));
				int chunkY = std::floor((viNextPosition->y / viChunkTileCount->y));
				int chunkIndex = chunkY * viChunkCount->y + chunkX;

				int tileX = std::fmodf(viNextPosition->x, (float)viChunkTileCount->x);
				int tileY = std::fmodf(viNextPosition->y, (float)viChunkTileCount->y);
				int tileIndex = (tileY * viChunkTileCount->y + tileX);

				if (vChunk[chunkIndex]->vTiles[tileIndex]->eTileType == Tree) {
					// collision. update nothing, return true;
					return true;
				}
				else {
					// no collision, continue rendering normal movement
					// save the positions for use when determining world position
					iChunkIndex = chunkIndex;
					iTileIndex = tileIndex;
					iChunkX = chunkX;
					iChunkY = chunkY;
					
					return false;
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
