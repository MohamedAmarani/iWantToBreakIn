#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
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

	scene.init();
	credits.init();
	menu.init();
	info.init();
	state = 0;

	playSoundBGM("sounds/getout.ogg");

}

bool Game::update(int deltaTime)
{
	if (state == 0)
		menu.update(deltaTime);
	else if (state == 1) 
		scene.update(deltaTime);
	else if (state == 2)
		info.update(deltaTime);
	else if (state == 3)
		credits.update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == 0)
		menu.render();
	else if (state == 1) {
		scene.render();
	}
	else if (state == 2)
		info.render();
	else if (state == 3)
		credits.render();
}

void Game::keyPressed(int key)
{
	//if(key == 27) // Escape code
	//bPlay = false;
	if(key == '1')
		SoundEngine->removeSoundSource("sounds/getout.ogg");
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
	state = s;
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
	SoundEngine->setSoundVolume(0.3);
}

void Game::playSound(const char * sound)
{
	ISound* aSong = SoundEngine->play2D(sound, false);
}
