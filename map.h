
using namespace std;

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

		tile(olc::vf2d position, olc::vi2d *tileSize, tiletype tile_type) {
			vfPosition = position;
			viTileSize = tileSize;
			eTileType = tile_type;
		}

		~tile() {
			delete viTileSize;
		}

		void Render(olc::PixelGameEngine* pge) {

			olc::Pixel colour;
			switch(eTileType) {
			case Grass:
				colour = olc::GREEN;
				break;

			case Tree:
				colour = olc::DARK_GREEN;
				break;

			case Sand:
				colour = olc::YELLOW;
				break;

			case Path:
				colour = olc::DARK_GREY;
				break;

			}

			pge->FillRect(vfPosition, olc::vi2d(viTileSize->x, viTileSize->y), colour);
		}

	};

	class chunk
	{
	public:
		olc::vf2d vfPosition;
		olc::vi2d *viChunkTileCount;
		olc::vi2d *viTileSize;

		std::vector<std::unique_ptr<tile>> vTiles;

		chunk(olc::vf2d position, olc::vi2d *tileCount, olc::vi2d *tileSize, std::string sChunk) {
			
			vfPosition = position;
			viChunkTileCount = tileCount;
			viTileSize = tileSize;

			GenerateTiles(sChunk);
		}

		~chunk() {
			delete viChunkTileCount;
			delete viTileSize;
		}

		void GenerateTiles(std::string sChunk)
		{
			for (int x = 0; x < viChunkTileCount->x; x++) {
				for (int y = 0; y < viChunkTileCount->y; y++) {
					tiletype eTileType;
					switch (sChunk[x * viChunkTileCount->y + y])
					{
					case '.':
						eTileType = Grass;
						break;

					case '|':
						eTileType = Tree;
						break;

					case '#':
						eTileType = Sand;
						break;

					case '+':
						eTileType = Path;
						break;

					default:
						eTileType = Grass;
					
					}				
					vTiles.push_back(std::make_unique<tile>(olc::vf2d(vfPosition.x + (x * viTileSize->x), vfPosition.y + (y * viTileSize->y)), viTileSize, eTileType));

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
	
    class MapGenerator {
        public:

			olc::vi2d *viChunkCount;
			olc::vi2d *viChunkTileCount;
			olc::vi2d *viTileSize;

			std::vector<std::unique_ptr<chunk>> vChunk;

			MapGenerator() {
                
            };

            ~MapGenerator() {
				delete viChunkCount;
				delete viChunkTileCount;
				delete viTileSize;
            };

			void GenerateChunks(olc::vi2d *chunkCount, olc::vi2d *tileCount, olc::vi2d *tileSize, std::string sChunk[])
			{

				viChunkCount = chunkCount;
				viChunkTileCount = tileCount;
				viTileSize = tileSize;
				
				for (int x = 0; x < chunkCount->x; x++) {
					for (int y = 0; y < chunkCount->y; y++) {
						vChunk.push_back(std::make_unique<chunk>(olc::vf2d(x * (viChunkTileCount->x * viTileSize->x), y * (viChunkTileCount->y * viTileSize->y)),
							tileCount, tileSize, sChunk[x * chunkCount->y + y]));
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
