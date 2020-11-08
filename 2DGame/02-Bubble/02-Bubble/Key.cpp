#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Key.h"
#include "Game.h"





enum KeyAnims
{
	SPINNING, EXPLODING, NOTHING
};


void Key::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/key_16_32.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1, 0.0833), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(SPINNING, 9);
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.5831f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.4998f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.4165f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.3332f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.2499f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.1666f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.0833f));
	sprite->addKeyframe(SPINNING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(EXPLODING, 4);
	sprite->addKeyframe(EXPLODING, glm::vec2(0.f, 0.6664f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.f, 0.7497f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.f, 0.833f));

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(0.f, 0.9167f));

	sprite->changeAnimation(NOTHING);
	tileMapDispl = tileMapPos;
}

void Key::update(int numKey, int deltaTime, int offset, int offsetR)
{
	if (!visible) {
		if (numKey == 1 && ((map->getOffset() == 2 && map->getOffseR() < 62) || (map->getOffset() == 1))) {
			sprite->setPosition(glm::vec2(100, (31 - (map->getOffseR() - 62)) * 16 - 750 + 17));
			int a = (31 - (map->getOffseR() - 93)) * 16 - 300;
			sprite->update(deltaTime);
			if (map->getOffset() == 1)
				if (sprite->animation() != 0)
					sprite->changeAnimation(SPINNING);
		}
		if (numKey == 2 && ((map->getOffset() == 3 && map->getOffseR() != 93) || (map->getOffset() == 2))) {
			sprite->setPosition(glm::vec2(246, (31 - (map->getOffseR() - 62)) * 16 - 300 + 17));
			int a = (31 - (map->getOffseR() - 62)) * 16 - 300;
			sprite->update(deltaTime);
			if (map->getOffset() == 2)
				if (sprite->animation() != 0)
					sprite->changeAnimation(SPINNING);
		}
		if (numKey == 3 && (map->getOffset() == 3)) {
			sprite->setPosition(glm::vec2(246, (31 - (map->getOffseR() - 62)) * 16 + 117));
			int a = (31 - (map->getOffseR() - 62)) * 16 - 300;
			sprite->update(deltaTime);
			if (map->getOffset() == 3)
				if (sprite->animation() != 0)
					sprite->changeAnimation(SPINNING);
		}
	}
	else {
		if (sprite->animation() == 1 && sprite->getKeyFrame() == 2)
			sprite->changeAnimation(NOTHING);

		if (!collision) {
			Game::instance().playSound("sounds/key.mp3");
			collision = true;
			sprite->changeAnimation(EXPLODING);
		}
	}
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

