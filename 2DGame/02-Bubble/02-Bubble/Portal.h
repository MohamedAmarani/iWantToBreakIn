#ifndef _PORTAL_INCLUDE
#define _PORTAL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Paddle is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Portal
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);


private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPortal;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int speed = 3;
	int count1 = 0;
};


#endif // _PADDLE_INCLUDE


