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


void Portal::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/portalWithBall.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
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
	sprite->addKeyframe(EAT_BALL, glm::vec2(0.9f, 0.75f));

	sprite->setAnimationSpeed(EXPULSE_BALL, 8);
	sprite->addKeyframe(EXPULSE_BALL, glm::vec2(0.9f, 0.75f));
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

	sprite->changeAnimation(EXPULSE_BALL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));

}

void Portal::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));
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


