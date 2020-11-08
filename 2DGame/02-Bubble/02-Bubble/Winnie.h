#ifndef _WINNIE_INCLUDE
#define _WINNIE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Winnie
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int offset, int offsetR, const glm::ivec2 &posWasp, bool chase, bool collision);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setStarted(bool b) { started = b; }
	glm::ivec2 getPosition() { return posPlayer; }

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int speed;
	bool started = false;
};


#endif // _WINNIE_INCLUDE