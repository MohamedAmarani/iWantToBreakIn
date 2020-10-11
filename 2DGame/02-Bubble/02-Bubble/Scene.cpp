#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 10
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 11.5
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	//to make right space
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH + 180), float(SCREEN_HEIGHT + 10), 0.f);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	// Select which font you want to use
	//if (!text.init("fonts/OpenSans-Regular.ttf"))
		if(!text.init("fonts/joystixMonospace.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
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
	map->render();
	player->render();

	//The vec4 is color
	text.render("MONEY:", glm::vec2(516, 40), 24, glm::vec4(1, 1, 1, 1));
	text.render("0000000", glm::vec2(502, 64), 24, glm::vec4(1, 1, 1, 1));

	text.render("POINTS:", glm::vec2(502, 136), 24, glm::vec4(1, 1, 1, 1));
	text.render("0000000", glm::vec2(502, 160), 24, glm::vec4(1, 1, 1, 1));

	text.render("LIVES:", glm::vec2(516, 232), 24, glm::vec4(1, 1, 1, 1));
	text.render("00", glm::vec2(576, 256), 24, glm::vec4(1, 1, 1, 1));

	text.render("BANK:", glm::vec2(532, 304), 24, glm::vec4(1, 1, 1, 1));
	text.render("00", glm::vec2(576, 328), 24, glm::vec4(1, 1, 1, 1));

	text.render("BATMODE", glm::vec2(502, 376), 24, glm::vec4(1, 1, 1, 1));
	text.render("SMALL", glm::vec2(520, 400), 24, glm::vec4(1, 1, 1, 1));

	text.render("ROOM:", glm::vec2(532, 436), 24, glm::vec4(1, 1, 1, 1));
	text.render("00", glm::vec2(576, 460), 24, glm::vec4(1, 1, 1, 1));
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



