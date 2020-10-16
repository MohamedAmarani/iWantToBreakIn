#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, STAND_UP, STAND_DOWN
};


void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	xSpeed = ySpeed = -1;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (map->collisionMoveUp(glm::ivec2(posPlayer.x, posPlayer.y - 1), glm::ivec2(16, 16), true))
		ySpeed = 1;
	if (map->collisionMoveDown(glm::ivec2(posPlayer.x, posPlayer.y + 1), glm::ivec2(16, 16), true))
		ySpeed = -1;
	if (map->collisionMoveLeft(glm::ivec2(posPlayer.x - 1, posPlayer.y), glm::ivec2(16, 16), true))
		xSpeed = 1;
	if (map->collisionMoveRight(glm::ivec2(posPlayer.x + 1, posPlayer.y), glm::ivec2(16, 16), true))
		xSpeed = -1;

	posPlayer.x += xSpeed;
	posPlayer.y += ySpeed;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


