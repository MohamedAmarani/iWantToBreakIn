#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
	winnie = NULL;
	ball = NULL;
	paddle = NULL;
	key1 = NULL;
	key2 = NULL;
	key3 = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	if (paddle != NULL)
		delete paddle;
}


void Scene::init(int level)
{
	initShaders();
	string myLevel = "levels/level0";
	village = 1;
	myLevel = myLevel + to_string(level) + ".txt";
	map = TileMap::createTileMap(myLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX()+8, INIT_PLAYER_Y_TILES * map->getTileSizeY()-16));
	ball->setTileMap(map);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(212, 400));
	player->setTileMap(map);
	winnie = new Winnie();
	winnie->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	winnie->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	winnie->setTileMap(map);
	key1 = new Key();
	key1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	key1->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	key1->setTileMap(map);
	key2 = new Key();
	key2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	key2->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	key2->setTileMap(map);
	key3 = new Key();
	key3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	key3->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	key3->setTileMap(map);
	paddle = new Paddle();
	paddle->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	paddle->setPosition(glm::vec2(212, 400));
	paddle->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	// Select which font you want to use
	//if (!text.init("fonts/OpenSans-Regular.ttf"))
	if (!text.init("fonts/joystixMonospace.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime, r);
	bool b = player->getDidStart();
	int xBee = player->getX();
	if (b)
		xBee = -1;
	if (bAnt && !b)
		bAnt = false;
	if (bAnt && b)
		b = false;
	if (!bAnt)
		bAnt = b;
	winnie->update(deltaTime, map->getOffset(), map->getOffseR(), player->getPosition(), chase);

	if (map->getKey()) {
		if (map->getOffset() == 1)
			key1->setVisible(true);
		else if (map->getOffset() == 2)
			key2->setVisible(true);
		else if (map->getOffset() == 3)
			key3->setVisible(true);
	}
	key1->update(1, deltaTime, map->getOffset(), map->getOffseR());
	key2->update(2, deltaTime, map->getOffset(), map->getOffseR());
	key3->update(3, deltaTime, map->getOffset(), map->getOffseR());
	paddle->update(deltaTime, r);
	glm::vec2 a = paddle->getPosition();

	if (map->getOffset() == 1)
		ball->update(deltaTime, a, b, xBee, key1->getVisible());
	else if (map->getOffset() == 2)
		ball->update(deltaTime, a, b, xBee, key2->getVisible());
	else if (map->getOffset() == 3)
		ball->update(deltaTime, a, b, xBee, key3->getVisible());
	else
		ball->update(deltaTime, a, b, xBee, key3->getVisible());

	bool restart = ball->getRestart();
	r = restart;

	if (restart)
		player->restartCount();

	if (Game::instance().getKey('3')) {
		chase = true;
	}

	if (map->getOffset() != 0) {
		chase = false;
	}

	if (!firstTime) {
		firstTime = true;
		Game::instance().playSoundBGM("sounds/scene.wav");
	}
	if (Game::instance().getKey(27)) { //ESC
		firstTime = false;
		Game::instance().playSoundBGM("sounds/summer.mp3");
		Game::instance().setState(0);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->prepareArrays(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->render();
	winnie->render();
	if (map->getOffset() == 1 || (map->getOffset() == 2 && map->getOffseR() < 62)) {
		key1->render();
	}
	if (map->getOffset() == 2 || (map->getOffset() == 3 && map->getOffseR() < 93)) {
		key2->render();
	}
	else if (map->getOffset() == 3) {
		key3->render();
	}
	player->render();
	ball->render();
	paddle->render();

	//The vec4 is color
	text.render("HONEY:", glm::vec2(508, 40), 24, glm::vec4(1, 1, 1, 1));

	string honeyValue = to_string(map->getHoney());
	string honeyZero = "";
	for (int i = honeyValue.length(); i < 7; ++i) {
		honeyZero += "0";
	}
	text.render((honeyZero + honeyValue), glm::vec2(484, 64), 24, glm::vec4(1, 1, 1, 1));

	text.render("POINTS:", glm::vec2(488, 136), 24, glm::vec4(1, 1, 1, 1));
	string pointValue = to_string(map->getPoints());
	string zero = "";
	for (int i = pointValue.length(); i < 7; ++i) {
		zero += "0";
	}
	text.render((zero + pointValue), glm::vec2(484, 160), 24, glm::vec4(1, 1, 1, 1));

	string livesValue = "0" + to_string(map->getLives());
	text.render("LIVES:", glm::vec2(506, 232), 24, glm::vec4(1, 1, 1, 1));
	text.render(livesValue, glm::vec2(580, 256), 24, glm::vec4(1, 1, 1, 1));

	string villageValue = "0" + to_string(village);
	text.render("VILLAGE:", glm::vec2(466, 304), 24, glm::vec4(1, 1, 1, 1));
	text.render(villageValue, glm::vec2(580, 328), 24, glm::vec4(1, 1, 1, 1));

	string houseValue = "0" + to_string(map->getHouse());

	text.render("HOUSE:", glm::vec2(506, 376), 24, glm::vec4(1, 1, 1, 1));
	text.render(houseValue, glm::vec2(580, 400), 24, glm::vec4(1, 1, 1, 1));

	text.render("GOD MODE:", glm::vec2(504, 440), 16, glm::vec4(1, 1, 1, 1));
	text.render("FALSE", glm::vec2(566, 456), 12, glm::vec4(1, 1, 1, 1));
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



