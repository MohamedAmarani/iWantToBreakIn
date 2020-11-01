#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Paddle.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PaddleAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, STAND_UP, STAND_DOWN
};


void Paddle::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/paddle32_8.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 8), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPaddle.x), float(tileMapDispl.y + posPaddle.y)));

}

void Paddle::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (posPaddle.y + 32) <= 27 * 16 - 2)
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPaddle.y += speed;
		posPaddle.x -= speed;
		if (map->collisionMoveDown(posPaddle, glm::ivec2(32, 32), 2) || map->collisionMoveLeft(posPaddle, glm::ivec2(32, 32), 2))
		{
			posPaddle.y -= speed;
			posPaddle.x += speed;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (posPaddle.y + 32) <= 27 * 16 - 2)
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPaddle.y += speed;
		posPaddle.x += speed;
		if (map->collisionMoveDown(posPaddle, glm::ivec2(32, 32), 2) || map->collisionMoveRight(posPaddle, glm::ivec2(32, 32), 2))
		{
			posPaddle.y -= speed;
			posPaddle.x -= speed;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_LEFT) && posPaddle.y >= 17)
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPaddle.y -= speed;
		posPaddle.x -= speed;
		if (map->collisionMoveUp(posPaddle, glm::ivec2(32, 32), 2) || map->collisionMoveLeft(posPaddle, glm::ivec2(32, 32), 2))
		{
			posPaddle.y += speed;
			posPaddle.x += speed;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && posPaddle.y >= 17)
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPaddle.y -= speed;
		posPaddle.x += speed;
		if (map->collisionMoveUp(posPaddle, glm::ivec2(32, 32), 2) || map->collisionMoveRight(posPaddle, glm::ivec2(32, 32), 2))
		{
			posPaddle.y += speed;
			posPaddle.x -= speed;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_UP) &&
			Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posPaddle.x -= speed;
			if (map->collisionMoveLeft(posPaddle, glm::ivec2(32, 32), 2))
			{
				posPaddle.x += speed;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getSpecialKey(GLUT_KEY_UP) &&
			Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posPaddle.x += speed;
			if (map->collisionMoveRight(posPaddle, glm::ivec2(32, 32), 2))
			{
				posPaddle.x -= speed;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && posPaddle.y >= 17)
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
			Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			posPaddle.y -= speed;
			if (map->collisionMoveUp(posPaddle, glm::ivec2(32, 32), 2))
			{
				posPaddle.y += speed;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && (posPaddle.y + 32) <= 27 * 16 - 2)
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
			Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			posPaddle.y += speed;
			if (map->collisionMoveDown(posPaddle, glm::ivec2(32, 32), 2))
			{
				posPaddle.y -= speed;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPaddle.x), float(tileMapDispl.y + posPaddle.y)));
}

void Paddle::render()
{
	sprite->render();
}

void Paddle::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Paddle::setPosition(const glm::vec2 &pos)
{
	posPaddle = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPaddle.x), float(tileMapDispl.y + posPaddle.y)));
}

glm::ivec2 Paddle::getPosition()
{
	return posPaddle;
}

