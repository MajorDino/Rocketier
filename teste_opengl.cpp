// Comando para compilação no terminal do VS Code
// g++ -o teste_opengl teste_opengl.cpp glut32.lib -lglu32 -lopengl32

#include <windows.h>
#include <iostream>
#include <gl/glut.h>
#include <math.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>

#include "Game.h"
#include "Player.h"
#include <iostream>

Game game;

int screenWidth = 800;
int screenHeight = 600;


float platformWidth = screenWidth;
float platformHeight = 50.0f;

float lifeBarContainerWidth = 200;
float lifeBarContainerHeight = 30;

float pointsTextPosX = 60;
float pointsTextPosY = screenHeight - 90.0f;

bool isPressed = false;
bool jump = false;
bool pausado = false;

bool inGame = false;

GLuint background_texture, rocket_texture, platform_texture, player_texture; // ID da textura do sprite

void setGameConfig();

void renderBitmapText(float x, float y, void *font, const char *string){
	glRasterPos2f(x, y);
	while (*string) {
		glutBitmapCharacter(font, *string);
		string++;
	}
}


bool testRetangularCollision(float paX, float paY, float aw, float ah, float pbX, float pbY, float bw, float bh) {
	return abs(paX-pbX) < (aw/2 + bw/2) && abs(paY-pbY) < (ah/2 + bh/2);
}

bool testRetangularCircularCollision(float paX, float paY, float aw, float ah, float pbX, float pbY, float r) {
	for(int i=0 ; i<360 ; i+=10) {
		float x = r*cos(i) + pbX, y = r*sin(i) + pbY;
		if(abs(paX - x) < aw/2 && abs(paY - y) < ah/2)
			return true;
	}
	return false;
}

void drawPlayer(Player p) {
	float width = p.width, height = p.height;
	glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-p.width/2, -p.height/2);
    glVertex2f(p.width/2, -p.height/2);
    glVertex2f(p.width/2, p.height/2);
    glVertex2f(-p.width/2, p.height/2);
    glEnd();
}

void drawLifeBarContainer(float width, float height) {
	glColor3f(0.1f, 0.1f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(width, 0.0f);
    glVertex2f(width+10.0f, height);
    glVertex2f(10.0f, height);
    glEnd();
}

void drawLifeBar(float width, float height) {
	glColor3f(0.1f, 0.7f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(width, 0.0f);
    glVertex2f(width+10.0f, height);
    glVertex2f(10.0f, height);
    glEnd();
}

void drawCoin(float radius) {
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
    for (int i = 0; i < 20; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / 20;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawPlatform(float width, float height) {
	glColor3f(0.196078f, 0.8f, 0.196078f);
	glBegin(GL_QUADS);
    glVertex2f(0.0f, height);
    glVertex2f(width, height);
    glVertex2f(width, height-50.0f);
    glVertex2f(0.0f, height - 50.0f);
    glEnd();
}

void drawRocket(float width, float height) {
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rocket_texture);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-width/2, -height/2); // Canto inferior esquerdo
    glTexCoord2f(1.0f, 0.0f); glVertex2f(width/2, -height/2); // Canto inferior direito
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width/2, height/2); // Canto superior direito
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-width/2, height/2); // Canto superior esquerdo
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void drawBackground(float width, float height) {
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f); // Canto inferior esquerdo
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, 0.0f); // Canto inferior direito
    glTexCoord2f(1.0f, 0.0f); glVertex2f(width, height); // Canto superior direito
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, height); // Canto superior esquerdo
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}



void drawMenu() {
	
}

void drawGameOver() {
	glColor3f(0.0f, 0.0f, 0.0f);
	const char *gameover = "GameOver";
    renderBitmapText(screenWidth/2-50.0f, screenHeight/2, GLUT_BITMAP_HELVETICA_18, gameover);
    
    glPushMatrix();
    glTranslatef(screenWidth/2-25.0f, screenHeight/2 - 24.0f, 0.0f);
    drawCoin(10);
    glPopMatrix();
    
    glColor3f(0.0f, 0.0f, 0.0f);
    char text[100];
    sprintf(text, "%d", (int) game.player.points);
    renderBitmapText(screenWidth/2-10.0f, screenHeight/2 - 30.0f, GLUT_BITMAP_HELVETICA_18, text);
    
    
    glColor3f(0.0f, 0.0f, 0.0f);
    const char* backToMenu = "Aperte barra de espaco para voltar ao Menu";
    renderBitmapText(screenWidth/2 - 160.0f, screenHeight/2 - 130.0f, GLUT_BITMAP_HELVETICA_18, backToMenu);
}

void drawGame() {
	glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f); // Posi��o da base
    drawBackground(screenWidth, screenHeight); // Largura e altura da base
    glPopMatrix();

    // Desenha o ret�ngulo horizontal (base)
    glPushMatrix();
    glTranslatef(game.player.posX, game.player.posY, 0.0f); // Posi��o da base
    drawPlayer(game.player);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    drawPlatform(platformWidth, platformHeight);
    glPopMatrix();
    
    // Rocket
	for(int i=0 ; i<game.rockets.size() ; ++i) {
		glPushMatrix();
	    glTranslatef(game.rockets[i].posX, game.rockets[i].posY, 0.0f);
	    drawRocket(100.0f, 50.0f);
	    glPopMatrix();
	}
	
	for(int i=0 ; i<game.coins.size() ; ++i) {
		glPushMatrix();
	    glTranslatef(game.coins[i].posX, game.coins[i].posY, 0.0f);
	    drawCoin(game.coins[i].radius);
	    glPopMatrix();
	}
	
	glPushMatrix();
    glTranslatef(30.0f, screenHeight - 50.0f, 0.0f); // Posi��o da base
    drawLifeBarContainer(lifeBarContainerWidth, lifeBarContainerHeight); // Largura e altura da base
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(30.0f, screenHeight - 50.0f, 0.0f); // Posi��o da base
    drawLifeBar(game.player.life, lifeBarContainerHeight); // Largura e altura da base
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(pointsTextPosX-20, pointsTextPosY+6, 0.0f); // Posi��o da base
    drawCoin(10); // Largura e altura da base
    glPopMatrix();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    char text[100];
    sprintf(text, "%d", (int) game.player.points);
    renderBitmapText(pointsTextPosX, pointsTextPosY, GLUT_BITMAP_HELVETICA_18, text);
}

void drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	switch(game.scene) {
		case MENU:
			drawMenu();
			break;
		case GAME:
			if(!inGame) setGameConfig();
			drawGame();
			break;
		case GAME_OVER:
			drawGameOver();
			break;
	}
	
	glutSwapBuffers();
}



void resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
   screenWidth = width;
   screenHeight = height;
   platformWidth = screenWidth;
}

void update(int value) {
	if(game.isGameOver()) return;
	if(!pausado) {
		game.movePlayer(isPressed, screenWidth, screenHeight, platformHeight);
		game.moveRockets();
		game.moveCoins();
		
		for(int i=0 ; i<game.rockets.size() ; ++i) {
			float rX = game.rockets[i].posX, rY = game.rockets[i].posY;
			float rW = game.rockets[i].width, rH = game.rockets[i].height;
			
			float pX = game.player.posX, pY = game.player.posY;
			float pW = game.player.width, pH = game.player.height;
			
			if(testRetangularCollision(pX, pY, pW, pH, rX, rY, rW, rH)) {
				game.remove_rocket(i);
				game.player.life -= 20.0f;
			}
			
			if(game.player.life <= 0) {
				game.setGameOver();
				inGame = false;
			}
		}
		
		for(int i=0 ; i<game.coins.size() ; ++i) {
			float rX = game.coins[i].posX, rY = game.coins[i].posY;
			float r = game.coins[i].radius;
			
			float pX = game.player.posX, pY = game.player.posY;
			float pW = game.player.width, pH = game.player.height;
			
			if(testRetangularCircularCollision(pX, pY, pW, pH, rX, rY, r)) {
				game.player.points += game.coins[i].value;
				game.remove_coin(i);
			}
		}
	
	    glutPostRedisplay();
	}
	
    glutTimerFunc(16, update, 0);
}



void keyboardFunc(unsigned char key, int x, int y) {
	if(game.isGame()) {
		switch (key) {
	        case 'p': case 'P':
	        	pausado = !pausado;
	        	break;
	        default:
	        	break;
	    }
	} else if(game.isGameOver()) {
    	switch(key) {
    		case ' ': game.setGame(); break;
		}
	}
	
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
	switch (key) {
    	case GLUT_KEY_UP:
			isPressed = true;
	        if(game.player.collidePlatform(platformHeight))
	        	game.player.setJump();
			break;
    }
	
	glutPostRedisplay();
}

void specialKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        isPressed = false;
    }
    
    glutPostRedisplay();
}


void addEnemies(int value) {
	if(game.isGameOver()) return;
	glutTimerFunc(1000, addEnemies, 0);
	if(pausado) return;
	game.addEnemie(screenWidth, screenHeight);
}

void addCoins(int value) {
	if(game.isGameOver()) return;
	glutTimerFunc(1000, addCoins, 0);
	if(pausado) return;
	game.addCoin(screenWidth, screenHeight);
}

void increaseSpeed(int value) {
	if(game.isGameOver()) return;
	if(game.rocketSpeed + 1.0f  <= 20.0f)
		glutTimerFunc(10000, increaseSpeed, 0);
	if(pausado) return;
	game.rocketSpeed += 1.0f;
}

void setGameConfig() {
	inGame = true;
	
	glutTimerFunc(0, update, 0);
    glutTimerFunc(3000, addEnemies, 0);
    glutTimerFunc(3000, addCoins, 0);
    glutTimerFunc(10000, increaseSpeed, 0);
}

void menuFunc(int option) {
    switch (option) {
        case 1:
            game.reset();
            break;
        case 2:
            glutDestroyWindow(glutGetWindow());
        	exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    
    glutCreateWindow("Game");
    
	glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    
    glutReshapeFunc(resize);
    glutDisplayFunc(drawScene);
    
    init("missile.png", rocket_texture);
    init("back.png", background_texture);
    
    int menuId = glutCreateMenu(menuFunc);
    glutAddMenuEntry("Reiniciar", 1);
    glutAddMenuEntry("Sair", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();
    return 0;
}
