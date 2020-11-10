#pragma once
#include "Credits.h"
#include "Text.h"

class Menu
{
public:
	Menu();
	~Menu();
	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
Texture menu;
TexturedQuad *fondo;
ShaderProgram texProgram;
float currentTime;
glm::mat4 projection;
Text texto;
int pos;
bool a;
bool b;
int cont = 0;
string password = "";
bool keys[256];
bool pressedSpace = true;
};

