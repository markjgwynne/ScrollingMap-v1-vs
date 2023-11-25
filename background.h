
using namespace std;

#include "olcPixelGameEngine.h"

#ifndef GAMEBACKGROUND_H_INCLUDED
#define GAMEBACKGROUND_H_INCLUDED

namespace CarRace 
{
    struct chunk_t
    {
        chunk_t() {

        }
        olc::vf2d m_vChunkPosition;
        
        virtual void moveTo(olc::vf2d new_location) {
            m_vChunkPosition = new_location;
        }

        virtual std::string getPosition() {
            return "X: " + std::to_string(m_vChunkPosition.x) + ", Y: " + std::to_string(m_vChunkPosition.y);
        }
        
    };
    
    class GameBackground {
        public:

            olc::vi2d vBlockSize = {64, 64};

            std::string sChunk =
                "...||......||..."
                "...||......||..."
                "...||......||..."
                "---++------++---"
                "---++------++---"
                "...||......||..."
                "...||......||..."
                "...||......||...";

            olc::vf2d vChunkSize = { 16, 8 };

            olc::vf2d vBackgroundSize;

            GameBackground() {
                
            };

            ~GameBackground() {

                delete sprLoaderSheetStreet;
                delete sprStreetHorizontal;
                delete sprStreetVertical;
                delete sprStreetCrossroad;
                delete sprGrass;
                delete sprBackground;
                delete decBackground;

            };

            void LoadBackground(olc::PixelGameEngine* pge, int chunkCountWidth, int chunkCountHeight) {
                
                LoadSpritesTiles(pge);

                CreateChunkObjects(pge, chunkCountWidth, chunkCountHeight);

                LoadBackgroundSprite(pge);

            };

            void RenderBackground(olc::PixelGameEngine* pge, olc::vf2d vCameraPos, int screenWidth, int screenHeight) {

                pge->DrawPartialDecal({ 0, 0 }, { (float)screenWidth, (float)screenHeight }, decBackground, vCameraPos, { (float)screenWidth, (float)screenHeight });
                    
            };
        
        private:
            olc::Sprite *sprLoaderSheetStreet = nullptr;
            olc::Sprite *sprStreetHorizontal = nullptr;
            olc::Sprite *sprStreetVertical = nullptr;
            olc::Sprite *sprStreetCrossroad = nullptr;
            olc::Sprite *sprGrass = nullptr;
            olc::Sprite *sprBackground = nullptr;
            olc::Decal *decBackground = nullptr;

            std::vector<std::unique_ptr<chunk_t>> vChunk;


            void LoadSpritesTiles(olc::PixelGameEngine* pge) {
                sprLoaderSheetStreet = new olc::Sprite("./gfx/streets.png");

                // Grass, pos={32, 32}, size={32, 32}
                sprGrass = new olc::Sprite(64, 64);
                pge->SetDrawTarget(sprGrass);
                pge->DrawPartialSprite({0, 0}, sprLoaderSheetStreet, {32, 32}, {32, 32});
                pge->DrawPartialSprite({32, 0}, sprLoaderSheetStreet, {32, 32}, {32, 32});
                pge->DrawPartialSprite({0, 32}, sprLoaderSheetStreet, {32, 32}, {32, 32});
                pge->DrawPartialSprite({32, 32}, sprLoaderSheetStreet, {32, 32}, {32, 32});
                
                // Street Horizontal, pos={8, 0}, size={32, 64}
                sprStreetHorizontal = new olc::Sprite(64, 64);
                pge->SetDrawTarget(sprStreetHorizontal);
                pge->DrawPartialSprite({0, 0}, sprLoaderSheetStreet, {128, 0}, {32, 64});
                pge->DrawPartialSprite({32, 0}, sprLoaderSheetStreet, {128, 0}, {32, 64});

                // Street Vertical, pos={2, 8}, size={64, 32}
                sprStreetVertical = new olc::Sprite(64, 64);
                pge->SetDrawTarget(sprStreetVertical);
                pge->DrawPartialSprite({0, 0}, sprLoaderSheetStreet, {32, 128}, {64, 32});
                pge->DrawPartialSprite({0, 32}, sprLoaderSheetStreet, {32, 128}, {64, 32});

                // Street Intersection Cross, pos={8, 4}, size={4, 4} 
                sprStreetCrossroad = new olc::Sprite(64, 64);
                pge->SetDrawTarget(sprStreetCrossroad);
                pge->DrawPartialSprite({0, 0}, sprLoaderSheetStreet, {128, 64}, {64, 64});
                //pge->DrawPartialSprite({0, 0}, sprLoaderSheetStreet, {96, 128}, {64, 64});

                // Don't foregt to set the draw target back to being the main screen (been there... wasted 1.5 hours :| )
                pge->SetDrawTarget(nullptr);
            };


            void CreateChunkObjects(olc::PixelGameEngine* pge, int chunkCountWidth, int chunkCountHeight) {
                /*
                    because we are iterating through x and y, we need  this calculation to get the chunk number: vChunk[x * chunkCountWidth + y]
                */

                for (int x = 0; x < chunkCountWidth; x++ ) {
                    for (int y = 0; y < chunkCountHeight; y++ ) {
                        
                        vChunk.push_back(std::make_unique<chunk_t>());
                        if((x * chunkCountWidth + y) == 0) {
                            vChunk[x * chunkCountWidth + y]->moveTo({ (float)x*vBlockSize.x, (float)y*vBlockSize.y });
                        } else {
                            vChunk[x * chunkCountWidth + y]->moveTo({ (float)(vChunkSize.x*x*vBlockSize.x), (float)(vChunkSize.y*y*vBlockSize.y) });
                        }
                    }
                }

                vBackgroundSize.x = vChunk[vChunk.size()-1]->m_vChunkPosition.x + (vChunkSize.x * vBlockSize.x);
                vBackgroundSize.y = vChunk[vChunk.size()-1]->m_vChunkPosition.y + (vChunkSize.y * vBlockSize.y);

            };

            void LoadBackgroundSprite(olc::PixelGameEngine* pge) {
                
                sprBackground = new olc::Sprite(vBackgroundSize.x, vBackgroundSize.y);
                pge->SetDrawTarget(sprBackground);

                string renderMessage = "";
                
                for (int a = 0; a < vChunk.size(); a++) {
                //for (int a = 1; a < 2; a++) {

                    auto& b = vChunk[a];

                    for (int y = 0; y < vChunkSize.y; y++)
                    {
                        for (int x = 0; x < vChunkSize.x; x++)
                        {
                            switch (sChunk[y * vChunkSize.x + x])
                            {
                            case '-':
                                //pge->DrawSprite((b->m_vChunkPosition.x+x)*vBlockSize.x, (b->m_vChunkPosition.y+y)*vBlockSize.y, sprStreetHorizontal);
                                pge->DrawSprite((b->m_vChunkPosition.x)+x*vBlockSize.x, (b->m_vChunkPosition.y)+y*vBlockSize.y, sprStreetHorizontal);
                                break;

                            case '|':
                                pge->DrawSprite((b->m_vChunkPosition.x)+x*vBlockSize.x, (b->m_vChunkPosition.y)+y*vBlockSize.y, sprStreetVertical);
                                break;
                            
                            case '+':
                                pge->DrawSprite((b->m_vChunkPosition.x)+x*vBlockSize.x, (b->m_vChunkPosition.y)+y*vBlockSize.y, sprStreetCrossroad);
                                break;

                            case '.':
                                pge->DrawSprite((b->m_vChunkPosition.x)+x*vBlockSize.x, (b->m_vChunkPosition.y)+y*vBlockSize.y, sprGrass);
                                break;
                            }
                        }
                    }

                    //renderMessage += "Rendering Chunk " + std::to_string(a) + " of " + std::to_string(vChunk.size()) + ", in position " + b->getPosition() + "\n";
                    
                }

                //pge->DrawString(1, 11, renderMessage, olc::WHITE);

                // Don't foregt to set the draw target back to being the main screen (been there... wasted 1.5 hours :| )
                pge->SetDrawTarget(nullptr);          

                decBackground = new olc::Decal(sprBackground);
                
            };
    };
}
#endif
