#pragma once
#include "Credits.h"
#include "Text.h"

class Win
{
public:
	Win();
	~Win();
	void init(int state);
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
	int pos = 0;
	bool a;
	bool b;
	int cont = 0;
	bool first = true;
	int level;
	string password = "";
};

