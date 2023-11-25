
using namespace std;

#include "olcPixelGameEngine.h"

#ifndef GAMECAR_H_INCLUDED
#define GAMECAR_H_INCLUDED

#define PI 3.14159265

namespace CarRace 
{
        
    class GameCar {
        public:

            olc::vf2d vCarPos = { 0.0f, 0.0f };

            int32_t fCarLength = 0.0f;
            int32_t fCarWidth = 0.0f;
            float fCarAngle = 0.0f;
            float fCarAngleIncrement = 200.0f;
            
            float fScale = 0.75f;
            
            float fAcceration = 0.0f;
            float fSpeed = 0.0f;
            float fAccerationIncrement = 1.0f;
            float fFriction = 0.5f;
            //float fDrag = 1.0f;
            float fTopSpeed = 0.5f;
            
            olc::Sprite *sprCar = nullptr;
            olc::Decal *decCar = nullptr;
            

            olc::vf2d CalculateAngledPoint(olc::vf2d pos, float length, double angle) {
                pos.x += length * cos(angle * PI / 180.0);
                pos.y += length * sin(angle * PI / 180.0);
                return pos;
            };

            GameCar() {

            };

            ~GameCar() {
                delete sprCar;
                delete decCar;
            };

            void LoadCar(olc::PixelGameEngine* pge, olc::vf2d startingPosition) {

                // Called once at the start, so create things here
                // Load the sprite
                sprCar = new olc::Sprite("./gfx/sports-car-sml.png");

                // Create decal of fragment
                decCar = new olc::Decal(sprCar);

                fCarWidth = sprCar->height;
		        fCarLength = sprCar->width;

                vCarPos = startingPosition;

            };

            olc::vf2d ProjectMovement(olc::PixelGameEngine* pge, float fElapsedTime) {

                if (pge->GetKey(olc::Key::W).bHeld && pge->GetKey(olc::Key::S).bHeld) {
                    // forwards and backwards
                    // regardless of drive or reverse, slow the car to a halt if both buttons are pressed.
                } else {
                    // forwards - accerate / drive
                    if (pge->GetKey(olc::Key::W).bHeld) fAcceration = fAccerationIncrement;

                    // backwards
                    if (pge->GetKey(olc::Key::S).bHeld) fAcceration = -fAccerationIncrement;

                    // rolling to a halt
                    if (pge->GetKey(olc::Key::W).bReleased) fAcceration = 0;

                    // rolling to a halt
                    if (pge->GetKey(olc::Key::S).bReleased) fAcceration = 0;

                }

                if (fAcceration > 0) {
                    // forward acceration
                    if (fSpeed >= 0) {
                        // forwards direction
                        fSpeed += fAcceration * fElapsedTime;
                    } else if (fSpeed < 0) {
                        // reverse direction
                        fSpeed -= fAcceration * fElapsedTime;
                    }
                } else if (fAcceration < 0) {
                    // reverse acceration
                    if (fSpeed <= 0) {
                        // reverse direction
                        fSpeed += fAcceration * fElapsedTime;
                    } else if (fSpeed > 0) {
                        // forwards direction
                        fSpeed -= fAcceration * fElapsedTime;
                    }
                }

                // friction and top speed
                if (fSpeed > 0) {
                    // forwards
                    if (fSpeed > fTopSpeed) fSpeed = fTopSpeed;
                    fSpeed -= fFriction * fElapsedTime;
                } else if (fSpeed < 0) {
                    // reverse
                    if (fSpeed < -fTopSpeed) fSpeed = -fTopSpeed;
                    fSpeed += fFriction * fElapsedTime;
                }

                if (fSpeed < 0.001 && fSpeed > -0.001 ) fSpeed = 0;
                
                // -----------------------------------------
                // left and right (simple verion for testing).
                if (pge->GetKey(olc::Key::A).bHeld) {
                    fCarAngle -= fCarAngleIncrement * fElapsedTime;
                }
                if (pge->GetKey(olc::Key::D).bHeld) {
                    fCarAngle += fCarAngleIncrement * fElapsedTime;
                }

                // calculate new player position
                return CalculateAngledPoint(vCarPos, fSpeed, fCarAngle);

            };

            void Render(olc::PixelGameEngine* pge, olc::vf2d vfOffset) {

                pge->DrawRotatedDecal(vfOffset, decCar, (fCarAngle*PI)/180, { (float)fCarLength*0.8f, (float)fCarWidth*0.5f }, { 1*fScale, 1*fScale });

                pge->DrawString(1, 11, "Player Pos, X: " + std::to_string(vCarPos.x+ vfOffset.x) + ", Y: " + std::to_string(vCarPos.y + vfOffset.y), olc::WHITE);

            };

    };
}

#endif