#ifndef Included_Player_H

#define Included_Player_H

#include <algorithm>
#include <iostream>

class Player {
	float acceleration;
	float gravity;
	bool jump;
	
	public:
		float posX, posY;
		float width, height;
		float life;
		float points;
		
		Player(float pX, float pY) {
			reset(pX, pY);
		}
		
		void reset(float pX, float pY) {
			posX = pX;
			posY = pY;
			width = 50.0f;
			height = 70.0f;
			acceleration = 0.0f;
			gravity = -9.0f;
			jump = false;
			life = 200.0f;
			points = 0.0f;
		}
		
		float move(int isPressing, float screenWidth, float screenHeight, float platformHeight) {
			if(jump) {
				acceleration = 5.0f;
			} else if (isPressing) {
		    	acceleration = std::max(gravity, (float)(acceleration + 0.4));
		    	
		    	if(posY + height/2 >= screenHeight) {
		    		acceleration = 0;
				}
		    } else {
		        if((posY-height/2) <= 0.0f) {
		    		acceleration = 0;
				} else if(posY + height/2 < screenHeight) {
					acceleration = std::max(gravity, (float)(acceleration - 0.4));
				} else {
					acceleration = -0.2;
				}
		    }
			
			if(!collidePlatform(platformHeight)) {
				posY = std::min(screenHeight-height/2, posY + acceleration);	
			} else if(isPressing && jump) {
				jump = false;
				posY = std::min(screenHeight-height/2, posY + acceleration);
			} else {
				acceleration = 0;
			}
			
			posY = std::max(posY, 50.0f);
		}
		
		void setJump() {
			jump = true;
		}
		
		bool collidePlatform(float platformHeight) {
			return posY - height/2 <= platformHeight;
		}
};

#endif
