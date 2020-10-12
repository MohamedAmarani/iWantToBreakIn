#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h>
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	xSpeed = ySpeed = -1;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0, 0.25), &spritesheet, &shaderProgram);
	//up-left coner point
	srand(time(NULL));
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	//direction = STOP;

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	Move();
	if (map->collisionMoveLeft(posBall, glm::ivec2(32, 32))) {
		changeDirection((eDir)((rand() % 3) + 4));
		Move();
		//xSpeed = 1;
	}

	if (map->collisionMoveRight(posBall, glm::ivec2(32, 32))) {
		//inicialize 4
		changeDirection((eDir)((rand() % 3) + 1));
		Move();
		//xSpeed = -1;
	}
	if (map->collisionMoveDown(posBall, glm::ivec2(32, 32))) {
		changeDirection(direction == DOWNRIGHT ? UPRIGHT : UPLEFT);
		Move();
		//ySpeed = 1;
	}

	//top collision
	if (map->collisionMoveUp(posBall, glm::ivec2(32, 32))) {
		changeDirection(direction == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
		Move();
		//ySpeed = -1;
	}

	/*posBall.x += xSpeed;
		posBall.y += ySpeed;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));*/
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
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::changeDirection(eDir d) {
	direction = d;
}

void Ball::randomDirection()
{
	direction = (eDir)((rand() % 6) + 1);
}

void Ball::Move()
{
	switch (direction)
	{
	case STOP:
		break;
	case LEFT:
		xSpeed = -1;
		break;
	case RIGHT:
		xSpeed = 1;
	case UPLEFT:
		xSpeed = -1;
		ySpeed = -1;
		break;
	case DOWNLEFT:
		xSpeed = -1;
		ySpeed = 1;
		break;
	case UPRIGHT:
		xSpeed = 1;
		ySpeed = -1;
		break;
	case DOWNRIGHT:
		xSpeed = 1;
		ySpeed = 1;
		break;
	default:
		break;
	}
	posBall.x += xSpeed;
	posBall.y += ySpeed;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

eDir Ball::getDirection()
{
	return direction;
}