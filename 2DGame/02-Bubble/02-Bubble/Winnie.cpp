#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Winnie.h"
#include "Game.h"





enum WinnieAnims
{
	STAND_DOWN, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, STAND_UP
};


void Winnie::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/winnie.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);


	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.1875f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.3225f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.4375f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.5625f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.6875f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.8125f, 0.f));
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.9375f, 0.f));

	sprite->changeAnimation(STAND_DOWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Winnie::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

