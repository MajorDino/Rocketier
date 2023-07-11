#include <vector>
#include "Rocket.h"
#include "Player.h"
#include "Coin.h"

enum GameState {
    MENU,
    GAME,
    GAME_OVER
};

class Game {
	public:
		Player player;
		GameState scene;
		std::vector<Rocket> rockets;
		std::vector<Coin> coins;
		short int removedRockets[100];
		short int removedCoins[100];
		
		float rocketSpeed;
		
		Game(): player(100.0f, 500.0f) {
			scene = GAME;
			rocketSpeed = 10.0f;
			memset(removedRockets, 0, sizeof(removedRockets));
			memset(removedCoins, 0, sizeof(removedCoins));
		}
		
		void reset() {
			rocketSpeed = 10.0f;
			memset(removedRockets, 0, sizeof(removedRockets));
			memset(removedCoins, 0, sizeof(removedCoins));
			rockets.clear();
			coins.clear();
			
			player.reset(100.0f, 500.0f);
		}
		
		void addEnemie(float screenWidth, float screenHeight) {
			Rocket rocket(screenWidth, screenHeight);
			rockets.push_back(rocket);
		}
		
		void addCoin(float screenWidth, float screenHeight) {
			Coin coin(screenWidth, screenHeight);
			coins.push_back(coin);
		}
		
		void moveRockets() {
			std::vector<Rocket> aux;
			for(int i=0 ; i<rockets.size() ; ++i) {
				if(rockets[i].posX + rockets[i].width > 0 && !removedRockets[i]) {
					rockets[i].move(rocketSpeed);
					aux.push_back(rockets[i]);
				}
				removedRockets[i] = 0;
			}
			rockets = std::vector<Rocket>(aux);
		}
		
		void moveCoins() {
			std::vector<Coin> aux;
			for(int i=0 ; i<coins.size() ; ++i) {
				if(coins[i].posX + coins[i].radius > 0 && !removedCoins[i]) {
					coins[i].move(8.0f);
					aux.push_back(coins[i]);
				}
				removedCoins[i] = 0;
			}
			coins = std::vector<Coin>(aux);
		}
		
		void remove_rocket(int pos) {
			removedRockets[pos] = 1;
		}
		
		void remove_coin(int pos) {
			removedCoins[pos] = 1;
		}
		
		void movePlayer(int isPressing, float screenWidth, float screenHeight, float platformHeight) {
			player.move(isPressing, screenWidth, screenHeight, platformHeight);
		}
		
		void setGameOver() {
			scene = GAME_OVER;
		}
		
		void setMenu() {
			scene = MENU;
		}
		
		void setGame() {
			scene = GAME;
			reset();
		}
		
		bool isGameOver() {
			return scene == GAME_OVER;
		}
		
		bool isMenu() {
			return scene == MENU;
		}
		
		bool isGame() {
			return scene == GAME;
		}
};
