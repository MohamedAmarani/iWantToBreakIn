#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	lives = 0;
	honey = 0;
	points = 0;
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	SoundEngine = createIrrKlangDevice();

	if (!SoundEngine)
	{
		printf("Could not startup engine\n");
		return; // error starting up the engine
	}

	if (CurrentPlayingSound)
		CurrentPlayingSound->drop();
	CurrentPlayingSound = 0;

	credits.init();
	menu.init();
	info.init();
	gameOver.init();
	state = 0;

	playSoundBGM("sounds/summer.mp3");

}

bool Game::update(int deltaTime)
{
	if (state == -1)
		win.update(deltaTime);
	else if (state == 0)
		menu.update(deltaTime);
	else if (state == 1) 
		gameOver.update(deltaTime);
	else if (state == 2)
		info.update(deltaTime);
	else if (state == 3)
		credits.update(deltaTime);
	else if (state > 3)
		scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == -1)
		win.render();
	else if (state == 0)
		menu.render();
	else if (state == 1) 
		gameOver.render();
	else if (state == 2)
		info.render();
	else if (state == 3)
		credits.render();
	else if (state > 3)
		scene.render();
}

void Game::keyPressed(int key)
{
	//if(key == 27) // Escape code
	//bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setState(int s) {
	if (state > 3)
		win.init(state);
	state = s;
	if (s > 3) {
		if (s > 4)
			scene.init(s - 3, honey, points, lives);
		else
			scene.init(s - 3, 0, 0, 4);
	}
}

void Game::playSoundBGM(const char * sound)
{

	// To play a sound, we only to call play2D(). The second parameter
	// tells the engine to play it looped.

	// play some sound stream, looped
	if (CurrentPlayingSound)
		CurrentPlayingSound->drop();
	SoundEngine->removeAllSoundSources();
	CurrentPlayingSound = SoundEngine->play2D(sound, true);
	SoundEngine->setSoundVolume(0.5);
}

void Game::playSound(const char * sound)
{
	ISound* aSound = SoundEngine->play2D(sound, false);
}

void  Game::playInterruptSound(const char* sound) {
	if (!SoundEngine->isCurrentlyPlaying(sound))
		interrupSound = SoundEngine->play2D(sound, true);
}

void Game::stopSound() {
	SoundEngine->removeSoundSource("sounds/snoring3.wav");
}

void Game::stopBGM() {
	SoundEngine->removeSoundSource("sounds/breakout.mp3");
}