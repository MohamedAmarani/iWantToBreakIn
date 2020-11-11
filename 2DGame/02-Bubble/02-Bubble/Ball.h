#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, const glm::vec2 &posPaddle, bool didStart, int xBee, bool visible, int offset, int offsetR);
	void render(int offset, int offsetR);

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() { return posPlayer; }
	bool getRestart();

private:
	bool bJumping, visible;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;
	int jumpAngle, startY;
	double xSpeed, ySpeed, xSpeedA, ySpeedA;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool restart = false;
};


#endif // _BALL_INCLUDE


