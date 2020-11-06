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

	spritesheet.loadFromFile("images/myWinnie.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 0.33333), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(SLEEPING, 2);
	sprite->addKeyframe(SLEEPING, glm::vec2(0.375f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.3125f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.25f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.1875f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.125f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.0625f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.f, 0.3333f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.9375f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.8125f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.6875f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.5625f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.4375f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.3125f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.1875f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(SLEEPING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(WAKING_UP, 7);
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
	sprite->addKeyframe(NOTHING, glm::vec2(0.9375f, 0.666f));

	sprite->changeAnimation(NOTHING);
	tileMapDispl = tileMapPos;
}

void Winnie::update(int deltaTime, int offset, int offsetR)
{
	int a = (31 - (map->getOffseR() - 62)) * 16 - 200;
	sprite->update(deltaTime);
	if ((map->getOffset() == 2 && map->getOffseR() != 62) || (map->getOffset() == 3 && map->getOffseR() != 93)) {
		sprite->setPosition(glm::vec2(200, (31 - (map->getOffseR() - 62)) * 16 - 200));
		sprite->changeAnimation(SLEEPING);
	}
	else if(map->getOffset() != 2)
		sprite->changeAnimation(NOTHING);
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

