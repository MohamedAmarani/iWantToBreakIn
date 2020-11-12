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
	visible = true;

	spritesheet.loadFromFile("images/grayBall.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Ball::update(int deltaTime, const glm::vec2 &posPaddle, bool didStart, int xBee, bool visibleK, int offset, int offsetR)
{
	if (didStart) {
		xSpeedA = xSpeed = 4.3329096379090570;
		ySpeedA = ySpeed = -3.6367422330589796;
	}
	if (xBee >= 0) {
		posPlayer.x = xBee + 8;
		posPlayer.y = 384;
		xSpeedA = xSpeed = 0;
		ySpeedA = ySpeed = 0;
	}

	//Seguir abeja

	sprite->update(deltaTime);
	double aux;
	double u = ySpeed;
	if (map->collisionMoveUp(glm::ivec2(posPlayer.x, posPlayer.y + ySpeed), glm::ivec2(16, 16), 1, visibleK))
		ySpeedA = ySpeed = ySpeed * (-1);
	if (map->collisionMoveDown(glm::ivec2(posPlayer.x, posPlayer.y + ySpeed), glm::ivec2(16, 16), 1, visibleK))
		ySpeedA = ySpeed = ySpeed * (-1);
	if ((aux = map->collisionMoveDownBallX(glm::ivec2(posPlayer.x, posPlayer.y + ySpeed), posPlayer, glm::ivec2(16, 16), 1, xSpeed, ySpeed)) != -100)
		xSpeedA = xSpeed = aux; 
	if ((aux = map->collisionMoveDownBallY(glm::ivec2(posPlayer.x, posPlayer.y + ySpeed), posPlayer, glm::ivec2(16, 16), 1, xSpeed, ySpeed)) != -100)
		ySpeedA = ySpeed = aux;
	if (map->collisionMoveLeft(glm::ivec2(posPlayer.x + xSpeed, posPlayer.y), glm::ivec2(16, 16), 1, visibleK))
		xSpeedA = xSpeed = xSpeed*(-1);
	if (map->collisionMoveRight(glm::ivec2(posPlayer.x + xSpeed, posPlayer.y), glm::ivec2(16, 16), 1, visibleK))
		xSpeedA = xSpeed = xSpeed*(-1);

	int a = map->amITr(posPlayer);

	if (a == 0) {
		visible = false;
		xSpeed = ySpeed = 0;
		posPlayer.y = 399;
	}
	else if (a == 1) {
		visible = false;
		xSpeed = ySpeed = 0;
		posPlayer.y = 34;
	}
	else if (a == 2){
		visible = true;
		xSpeed = xSpeedA;
		ySpeed = ySpeedA;
	}
	else if (a == 4) {
		restart = true;
	}
	if ((offset == 0 && offsetR == 0) || (offset == 1 && offsetR == 31) || (offset == 2 && offsetR == 62) || (offset == 3 && offsetR == 93)) {
			posPlayer.x += xSpeed;
			posPlayer.y += ySpeed;
	}

	if (((offset == 0 && offsetR == 0 && ySpeed == 0) || (offset == 1 && offsetR == 31 && ySpeed == 0) 
		|| (offset == 2 && offsetR == 62 && ySpeed == 0) || (offset == 3 && offsetR == 93 && ySpeed == 0)) && visible) {
		xSpeedA = xSpeed = 4.3329096379090570;
		ySpeedA = ySpeed = -3.6367422330589796;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Ball::render(int offset, int offsetR)
{
	if(visible)
		if ((offset == 0 && offsetR == 0) || (offset == 1 && offsetR == 31) || (offset == 2 && offsetR == 62) || (offset == 3 && offsetR >= 93) || (xSpeed == 0 && ySpeed == 0))
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


bool Ball::getRestart()
{
	bool aux = restart;
	if (restart)
		restart = false;
	return aux;
}