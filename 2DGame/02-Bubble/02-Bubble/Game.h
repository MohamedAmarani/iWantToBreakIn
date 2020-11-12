#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Info.h"
#include "Scene.h"
#include "Credits.h"
#include "Menu.h"
#include "GameOver.h"
#include "Win.h"

#include <stdio.h>
#include <irrKlang.h>
#include <ik_ISound.h>
#include <conio.h>
using namespace irrklang;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}


	static Game &instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void setState(int state);
	void setHoney(int h) { honey = h; };
	int getHoney() { return honey; };
	void setPoints(int p) { points = p; };
	void setLives(int l) { lives = l; };

	void playSoundBGM(const char* sound);
	void playSound(const char* sound);
	void playInterruptSound(const char* sound);
	void Game::stopSound();
	void Game::stopBGM();

private:
	Win win;
	GameOver gameOver;
	Info info;
	Menu menu;
	int state;
	Credits credits;
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time
	ISoundEngine* SoundEngine;
	ISound* CurrentPlayingSound;
	ISound* interrupSound;
	int honey;
	int points;
	int lives;
};


#endif // _GAME_INCLUDE


