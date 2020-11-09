#ifndef _PADDLE_INCLUDE
#define _PADDLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Paddle is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Paddle
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, bool restart, bool collision, int offset, int offsetR, bool backing);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	glm::ivec2 Paddle::getPosition();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPaddle;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int speed = 3;
	int count1 = 0;
};


#endif // _PADDLE_INCLUDE


