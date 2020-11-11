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
	portal = NULL;
	key1 = NULL;
	key2 = NULL;
	key3 = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	if (paddle != NULL)
		delete paddle;
	if (portal != NULL)
		delete portal;
}


void Scene::init(int level)
{
	Olevel = level;
	initShaders();
	string myLevel = "levels/level0";
	village = level;
	myLevel = myLevel + to_string(level) + ".txt";
	map = TileMap::createTileMap(myLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setLevel(village);
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX() + 8, INIT_PLAYER_Y_TILES * map->getTileSizeY() - 16));
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
	portal1 = new Portal();
	portal1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
	portal1->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal1->setTileMap(map);
	portal2 = new Portal();
	portal2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2);
	portal2->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal2->setTileMap(map);
	portal3 = new Portal();
	portal3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3);
	portal3->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal3->setTileMap(map);
	portal4 = new Portal();
	portal4->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 4);
	portal4->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal4->setTileMap(map);
	portal5 = new Portal();
	portal5->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 5);
	portal5->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal5->setTileMap(map);
	portal6 = new Portal();
	portal6->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 6);
	portal6->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	portal6->setTileMap(map);
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
	if (map->getHitHoney() == 18 && Olevel == 1) {
		Game::instance().setState(-1); //YOU WIN
		map->resetHitHoney();
	}
	else if (map->getHitHoney() == 12 && Olevel == 2) {
		Game::instance().setState(1); //YOU WIN
		map->resetHitHoney();
	}
	else if (map->getHitHoney() == 30 && Olevel == 3) {
		Game::instance().setState(1); //YOU WIN
		map->resetHitHoney();
	}

	if (map->getLives() == 0)
		Game::instance().setState(1); //GAME OVER

	glm::vec2 posAux = ball->getPosition();

	//if(map->getOffset() == 3 && map->getOffseR() == 93 && pos)
		
	portal1->update(deltaTime, 1, map->getOffset(), map->getOffseR(), false, false);
	portal2->update(deltaTime, 2, map->getOffset(), map->getOffseR(), false, false);
	portal3->update(deltaTime, 3, map->getOffset(), map->getOffseR(), false, false);
	portal4->update(deltaTime, 4, map->getOffset(), map->getOffseR(), false, false);
	portal5->update(deltaTime, 5, map->getOffset(), map->getOffseR(), false, false);
	portal6->update(deltaTime, 6, map->getOffset(), map->getOffseR(), false, false);

	currentTime += deltaTime;
	player->update(deltaTime, r, collision, map->getOffset(), map->getOffseR());
	bool b = player->getDidStart();
	bool ba = player->getBacking();
	int xBee = player->getX();
	if (b)
		xBee = -1;
	if (bAnt && !b)
		bAnt = false;
	if (bAnt && b)
		b = false;
	if (!bAnt)
		bAnt = b;
	winnie->update(deltaTime, map->getOffset(), map->getOffseR(), player->getPosition(), chase, collision);

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
	paddle->update(deltaTime, r, collision, map->getOffset(), map->getOffseR(), ba);
	glm::vec2 a = paddle->getPosition();
	glm::vec2 w = winnie->getPosition();

	if (map->getOffset() == 1)
		ball->update(deltaTime, a, b, xBee, key1->getVisible(), map->getOffset(), map->getOffseR());
	else if (map->getOffset() == 2)
		ball->update(deltaTime, a, b, xBee, key2->getVisible(), map->getOffset(), map->getOffseR());
	else if (map->getOffset() == 3)
		ball->update(deltaTime, a, b, xBee, key3->getVisible(), map->getOffset(), map->getOffseR());
	else
		ball->update(deltaTime, a, b, xBee, key3->getVisible(), map->getOffset(), map->getOffseR());

	bool restart = ball->getRestart();

	//colision winnie y player
	if (chase && map->getOffset() == 0 && map->getOffseR() == 0 && (a.x < w.x + 32) && (a.x + 32 > w.x) && (a.y < w.y + 32) && (32 + a.y > w.y)) {
		collision = true;
		restart = true;
		map->setChase(false);
		map->decrementLives();
		//winnie vuelve a dormir
		winnie->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		winnie->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
		winnie->setTileMap(map);
		++count;
		winnie->setStarted(false);
		Game::instance().playSound("sounds/die.mp3");
		map->setHouse(1);

	}
	else
		collision = false;
	if (count > 0)
		++count;

	r = restart;

	if (restart) {
		player->setDidStart(false);
		if (count == 100 || count == 0)
			player->restartCount();
	}

	if (count == 100) {
		map->setOffset(3);
		count = 0;
	}

	if (map->getChase()) {
		chase = true;
	}

	if (map->getOffset() != 0) {
		chase = false;
	}

	if (Game::instance().getKey('u') && !pressedU && player->getDidStart()) {
		pressedU = true;
		if ((map->getOffset() != 0)) {
			map->setOffset(map->getOffset() - 1);
			map->setHouse(map->getHouse() + 1);
		}
	}
	else if (!Game::instance().getKey('u')) {
		pressedU = false;
	}

	if (Game::instance().getKey('g') && !pressedG) {
		pressedG = true;
		if (!s) {
			s = true;
			map->setS(true);
		}
		else {
			s = false;
			map->setS(false);
		}
	}
	else if (!Game::instance().getKey('g')) {
		pressedG = false;
	}

	if (!firstTime) {
		firstTime = true;
		Game::instance().playSoundBGM("sounds/breakout.mp3");
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
	ball->render(map->getOffset(), map->getOffseR());
	paddle->render();
	portal1->render();
	portal2->render();
	portal3->render();
	portal4->render();
	portal5->render();
	portal6->render();
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

	/*string houseValue = "0" + to_string(map->getHoneyLevel3());

	text.render("HOUSE:", glm::vec2(506, 376), 24, glm::vec4(1, 1, 1, 1));
	text.render(houseValue, glm::vec2(580, 400), 24, glm::vec4(1, 1, 1, 1));*/

	text.render("GOD MODE:", glm::vec2(504, 440), 16, glm::vec4(1, 1, 1, 1));
	if (!s)
		text.render("FALSE", glm::vec2(566, 456), 12, glm::vec4(1, 1, 1, 1));
	else
		text.render("TRUE", glm::vec2(566, 456), 12, glm::vec4(1, 1, 1, 1));
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}