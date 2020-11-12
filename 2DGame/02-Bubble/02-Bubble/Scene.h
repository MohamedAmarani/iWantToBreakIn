#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Paddle.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Text.h"
#include "Winnie.h"
#include "Key.h"
#include "Portal.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level, int honey, int points, int lives);
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Portal *portal1, *portal2, *portal3, *portal4, *portal5, *portal6;
	Key *key1, *key2, *key3;
	Winnie *winnie;
	TileMap *map;
	Player *player;
	Ball *ball;
	Paddle *paddle;
	Portal *portal;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	int k = 8;
	bool firstTime = false;
	int village = 1;
	bool bAnt = false;
	bool r = false;
	bool chase = false;
	bool pressedU = true;
	bool pressedG = true;
	bool collision = false;
	int count = 0;
	bool s = false;
	int countHoney = 0;
	int Olevel = 0;
};


#endif // _SCENE_INCLUDE