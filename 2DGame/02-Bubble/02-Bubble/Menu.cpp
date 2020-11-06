#include "Menu.h"
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

Menu::Menu()
{

}

Menu::~Menu()
{

}


void Menu::init()
{
	initShaders();
	currentTime = 0.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	menu.loadFromFile("images/background2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pos = 0;
	texto.init("fonts/joystixMonospace.ttf");
	a = false;
	b = false;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;

	if (Game::instance().getKey(13)) //ENTER
		if (password == "EASY")
			Game::instance().setState(3 + 2);
		else if (password == "ANGRY")
			Game::instance().setState(3 + 3);
		else
			password = "";

	if (Game::instance().getKey(112) || Game::instance().getKey(80)) { //P
		if (pos == 0)
			keys[80] = false;
		pos = 1;
		cont = 0;
	}
	if (Game::instance().getKey(32)) { //SPACE
		Game::instance().setState(3 + 1);
	}
	if (Game::instance().getKey(27)) { //ESC
		if (pos == 1) {
			pos = 0;
			cont = 0;
			password = "";
		}
	}

	if (pos == 1) { //leer input de password
		for (int i = 65; i <= 90; ++i)
			if (Game::instance().getKey(i) && keys[i]) {
				keys[i] = false;
				if (password.size() < 4)
					password += i;
			}
			else if (!Game::instance().getKey(i))
				keys[i] = true;
		for (int i = 97; i <= 122; ++i)
			if (Game::instance().getKey(i) && keys[i]) {
				keys[i] = false;
				if (password.size() < 4)
					password += i - 32;
			}
			else if (!Game::instance().getKey(i))
				keys[i] = true;
	}
}

void Menu::render()
{

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	fondo->render(menu);
	++cont;
	if (pos == 0) {
		if (cont >= 20) {
			texto.render("PRESS FIRE TO START", glm::vec2(170, 400), 20, glm::vec4(1, 1, 1, 1));
		}
		if (cont >= 50)
			cont = 0;
		texto.render("OR P FOR PASSWORD", glm::vec2(186, 420), 20, glm::vec4(1, 1, 1, 1));
	}
	else {
		texto.render("ENTER PASSWORD : " + password, glm::vec2(130, 400), 20, glm::vec4(1, 1, 1, 1));
		if (cont >= 20) {
			texto.render("_", glm::vec2(415 + password.size()*16, 400), 20, glm::vec4(1, 1, 1, 1));
		}
		if (cont >= 50)
			cont = 0;
	}
}


void Menu::initShaders()
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