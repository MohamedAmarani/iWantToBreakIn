#include "GameOver.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Credits.h"
#include "Game.h"
#include <GL/glut.h>



#define SCREEN_X 32
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}


void GameOver::init()
{
	initShaders();
	currentTime = 0.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	menu.loadFromFile("images/go.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pos = 0;
	texto.init("fonts/joystixMonospace.ttf");
	a = false;
	b = false;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	Game::instance().playSoundBGM("sounds/gameover.mp3");
}

void GameOver::update(int deltaTime)
{
	currentTime += deltaTime;

	if (first) {
		Game::instance().playSoundBGM("sounds/gameover.mp3");
		first = false;
	}

	if (Game::instance().getKey(32)) { //SPACE
		Game::instance().setState(0);
		first = true;
	}
	++cont;
}

void GameOver::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	fondo->render(menu);
	if(cont >= 0 && cont <=40)
		texto.render("PRESS [SPACE] TO GET BACK TO THE MENU", glm::vec2(135, 440), 12, glm::vec4(1, 1, 0, 1));
	if (cont == 70)
		cont = 0;
}


void GameOver::initShaders()
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