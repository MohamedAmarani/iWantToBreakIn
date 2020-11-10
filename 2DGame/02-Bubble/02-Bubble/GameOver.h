#pragma once
#include "Credits.h"
#include "Text.h"

class GameOver
{
public:
	GameOver();
	~GameOver();
	void init();
	void update(int deltaTime);
	void render();
	void setReleased(bool b);
	void setReleasedESC(bool b);

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
};

