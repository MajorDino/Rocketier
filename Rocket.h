#ifndef Included_Rocket_H

#define Included_Rocket_H

#include "utils.h"
#include <algorithm>

class Rocket {
	public:
		float posX, posY;
		float width, height;
		char file_image[100];
		
		Rocket(float screenWidth, float screenHeight) {
			posX = screenWidth;
			posY = getRandomY(screenHeight-height);
			width = 100.0f;
			height = 50.0f;
		}
		
		float move(float speed) {
			posX -= speed;
		}
		
		float getRandomY(float maxY) {
			return getRandomFloat(50.0f + height, maxY);
		}
};

#endif
