#ifndef Included_Coin_H

#define Included_Coin_H

#include "utils.h"
#include <algorithm>

class Coin {
	public:
		float value;
		float posX, posY;
		float radius;
		char file_image[100];
		
		Coin(float screenWidth, float screenHeight) {
			posX = screenWidth;
			posY = getRandomY(screenHeight);
			radius = 20.0f;
			value = 1.0f;
		}
		
		float move(float speed) {
			posX -= speed;
		}
		
		float getRandomY(float maxY) {
			return getRandomFloat(50.0f+2*radius, maxY-2*radius);
		}
};

#endif
