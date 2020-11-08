#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Winnie.h"
#include "Game.h"





enum WinnieAnims
{
	SLEEPING, WAKING_UP, NOTHING, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, STAND_UP
};


void Winnie::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/myWinnie2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(SLEEPING, 2);
	sprite->addKeyframe(SLEEPING, glm::vec2(0.375f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.3125f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.1875f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.0625f, 0.25f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(WAKING_UP, 5);
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.9375f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.8125f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.6875f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.5625f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.4375f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.3125f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.1875f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(WAKING_UP, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(NOTHING, 7);
	sprite->addKeyframe(NOTHING, glm::vec2(0.9375f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3125f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4375f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.50f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.50f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.50f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.50f));

	sprite->changeAnimation(NOTHING);
	tileMapDispl = tileMapPos;
	speed = 0;
}

void Winnie::update(int deltaTime, int offset, int offsetR, const glm::ivec2 &posWasp, bool chase, bool collision)
{

	int a = (31 - (map->getOffseR() - 62)) * 16 - 200;
	sprite->update(deltaTime);

	if(chase && (map->getOffset() == 0) && map->getOffseR() == 0) {

		if (sprite->animation() == 0)
			sprite->changeAnimation(WAKING_UP);
		else if (started || (sprite->animation() == 1 && sprite->getKeyFrame() == 14) || (sprite->animation() == 3 || sprite->animation() == 4)) {
			started = true;
			Game::instance().stopSound();
			float speedX = posWasp.x - posPlayer.x;
			float speedY = posWasp.y - posPlayer.y;
			float maxSpeed = 2 * 0.5f;
			if (speedX > 0.f) {
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else {
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			if (speedX > maxSpeed)
				speedX = maxSpeed;
			if (speedX < -maxSpeed)
				speedX = -maxSpeed;

			if (speedY > maxSpeed)
				speedY = maxSpeed;
			if (speedY < -maxSpeed)
				speedY = -maxSpeed;

			posPlayer.x += speedX;
			posPlayer.y += speedY;
			sprite->setPosition(glm::vec2(posPlayer.x, tileMapDispl.y + posPlayer.y));
		}
	}

	else {
		if ((map->getOffset() == 0 && map->getOffseR() != 0) || (map->getOffset() == 1 && map->getOffseR() != 31)) {
			setPosition(glm::vec2(206, (31 - (map->getOffseR())) * 16 - 203));
			sprite->setPosition(glm::vec2(206, (31 - (map->getOffseR())) * 16 - 203));
			if(map->getOffset() == 0 && (map->getOffset() == 0))
				Game::instance().playInterruptSound("sounds/snoring.wav");
			sprite->changeAnimation(SLEEPING);
		}
		else if (map->getOffset() != 0)
			sprite->changeAnimation(NOTHING);
	}
}


void Winnie::render()
{
	sprite->render();
}

void Winnie::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Winnie::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
}

