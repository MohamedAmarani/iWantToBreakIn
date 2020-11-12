#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Portal.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PortalAnims
{
	IDLE, EAT_BALL, EXPULSE_BALL, NOTHING
};


void Portal::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int n)
{
	num = n;
	bJumping = false;
	spritesheet.loadFromFile("images/portalWithBall (2).png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(50, 50), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);


	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.1f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.3f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.7f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.9f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.1f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.3f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.7f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8f, 0.25f));
	sprite->addKeyframe(IDLE, glm::vec2(0.9f, 0.25f));

	sprite->setAnimationSpeed(EAT_BALL, 8);
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.3f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.9f, 0.5f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.1f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.3f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.6f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.8f, 0.75f));

	sprite->setAnimationSpeed(EXPULSE_BALL, 8);
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.6f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.3f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.1f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.9f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.3f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(0.9f, 0.75f));

	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));

}

void Portal::update(int deltaTime, int numPortal, int offset, int offsetR, bool enter, bool exit) {

	if (enter)
		sprite->changeAnimation(EAT_BALL);
	else if (exit)
		sprite->changeAnimation(EXPULSE_BALL);
	else if ((numPortal == 1 || numPortal == 2) && ((map->getOffset() == 2 && map->getOffseR() < 62) || (map->getOffset() == 1) || (map->getOffset() == 0 && map->getOffseR() > 0))) {
			if(sprite->animation() == 3)
				sprite->changeAnimation(IDLE);
			if (numPortal == 1)
				sprite->setPosition(glm::vec2(130, (31 - (map->getOffseR() - 62)) * 16 - 750 - 100));
			else
				sprite->setPosition(glm::vec2(295, (31 - (map->getOffseR() - 62)) * 16 - 750 + 87));
			sprite->update(deltaTime);
		}
	else if ((numPortal == 3 || numPortal == 4) && ((map->getOffset() == 3 && map->getOffseR() != 93) || (map->getOffset() == 2) || (map->getOffset() == 1 && map->getOffseR() > 31))) {
			if (sprite->animation() == 3)
				sprite->changeAnimation(IDLE);
			if (numPortal == 3)
				sprite->setPosition(glm::vec2(55, (31 - (map->getOffseR() - 62)) * 16 - 300 + 137));
			else
				sprite->setPosition(glm::vec2(360, (31 - (map->getOffseR() - 62)) * 16 - 300 + 137));
			sprite->update(deltaTime);
		}
	else if ((numPortal == 5 || numPortal == 6) && ((map->getOffset() == 3) || (map->getOffset() == 2 && map->getOffseR() > 62))) {
			if (sprite->animation() == 3)
				sprite->changeAnimation(IDLE);
			if (numPortal == 5)
				sprite->setPosition(glm::vec2(246, (31 - (map->getOffseR() - 62)) * 16 + 60));
			else
				sprite->setPosition(glm::vec2(100, (31 - (map->getOffseR() - 62)) * 16 + 370));
			sprite->update(deltaTime);
		}
	else
		sprite->changeAnimation(NOTHING);
}

void Portal::render()
{
	sprite->render();
}

void Portal::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Portal::setPosition(const glm::vec2 &pos)
{
	posPortal = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));
}

void Portal::setAnimation(int a)
{
	sprite->changeAnimation(a);
}

bool Portal::stoppedEating()
{
	return (sprite->animation() == 1 && sprite->getKeyFrame() == 18);
}

bool Portal::stoppedExpulsing()
{
	return (sprite->animation() == 2 && sprite->getKeyFrame() == 18);
}