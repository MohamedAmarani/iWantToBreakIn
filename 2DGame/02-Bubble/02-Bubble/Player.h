#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, bool restart, bool collision);
	void render();
	bool getDidStart();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	int getX();
	void restartCount();
	glm::ivec2 Player::getPosition();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int speed = 3;
	bool didStart = false;
	int count = 0;
};


#endif // _PLAYER_INCLUDE


