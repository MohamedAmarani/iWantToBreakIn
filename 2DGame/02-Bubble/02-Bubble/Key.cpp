#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Key.h"
#include "Game.h"





enum KeyAnims
{
	SPINNING, NOTHING
};


void Key::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/key16_32.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(SPINNING, 8);
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.875f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.625f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.375f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.125f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(1.f, 1.f));

	sprite->changeAnimation(NULL);
	tileMapDispl = tileMapPos;
}

void Key::update(int deltaTime, int offset, int offsetR)
{
	sprite->setPosition(glm::vec2(246, (31 - (map->getOffseR() - 62)) * 16 - 300 +17));
	int a = (31 - (map->getOffseR() - 62)) * 16 - 300;
	sprite->update(deltaTime);
	if ((map->getOffset() == 2 && map->getOffseR() != 62) || (map->getOffset() == 3 && map->getOffseR() != 93) || (map->getOffset() == 1 && map->getOffseR() != 31)) {
		if (((31 - (map->getOffseR() - 62)) * 16 - 300) > 15 && ((31 - (map->getOffseR() - 62)) * 16 - 300) < 27 * 16) {
			sprite->setPosition(glm::vec2(230, (31 - (map->getOffseR() - 62)) * 16 - 300));
			sprite->changeAnimation(SPINNING);
		}
	}
	else if (map->getOffset() != 2)
		sprite->changeAnimation(NULL);
}


void Key::render()
{
	sprite->render();
}

void Key::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Key::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
}

