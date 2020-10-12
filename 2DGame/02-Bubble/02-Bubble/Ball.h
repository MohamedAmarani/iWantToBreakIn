#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum eDir
{
	STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6
};

class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void Ball::changeDirection(eDir d);
	void Ball::randomDirection();
	void Ball::Move();
	eDir Ball::getDirection();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posBall;
	int jumpAngle, startY, xSpeed, ySpeed;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	//ball direction
	eDir direction;
	bool first = false;

};


#endif // _BALL_INCLUDE


