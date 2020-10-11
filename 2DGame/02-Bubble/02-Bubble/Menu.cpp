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
	menu.loadFromFile("images/menu.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	pos = 0;
	texto.init("fonts/DroidSerif.ttf");
	a = false;
	b = false;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;

	if (Game::instance().getKey(13)) //ENTER
		Game::instance().setState(pos + 1);

	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) { //UP
		if (pos > 0 && !a) {
			--pos;
		}
		else if (!a)
			pos = 2;
		a = true;
	}
	else 
		a = false;
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) { //DOWN
		if (pos < 2 && !b) {
			++pos;
		}
		else if (!b)
			pos = 0;
		b = true;
	}
	else
		b = false;
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
	if (pos == 0) {
		texto.render("PLAY ", glm::vec2(230, 130), 70, glm::vec4(1, 0.5, 1, 1));
		texto.render("INSTRUCTIONS ", glm::vec2(120, 250), 55, glm::vec4(1, 0.5, 1, 1));
		texto.render("CREDITS ", glm::vec2(210, 370), 55, glm::vec4(1, 0.5, 1, 1));
	}
	else if (pos == 1) {
		texto.render("PLAY ", glm::vec2(250, 130), 55, glm::vec4(1, 0.5, 1, 1));
		texto.render("INSTRUCTIONS ", glm::vec2(65, 250), 70, glm::vec4(1, 0.5, 1, 1));
		texto.render("CREDITS ", glm::vec2(210, 370), 55, glm::vec4(1, 0.5, 1, 1));
	}
	else if (pos == 2) {
		texto.render("PLAY ", glm::vec2(250, 130), 55, glm::vec4(1, 0.5, 1, 1));
		texto.render("INSTRUCTIONS ", glm::vec2(120, 250), 55, glm::vec4(1, 0.5, 1, 1));
		texto.render("CREDITS ", glm::vec2(180, 370), 70, glm::vec4(1, 0.5, 1, 1));
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