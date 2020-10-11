#ifndef _INFO_INCLUDE
#define _INFO_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Info
{

public:
	Info();
	~Info();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Texture info;
	TexturedQuad *fondo;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _INFO_INCLUDE