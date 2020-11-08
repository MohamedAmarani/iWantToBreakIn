#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	int getOffset();
	void setOffset(int off);

	int getOffseR();

	void render() const;
	void free();
	
	int getTileSizeX() const { return tileSize.x; }

	int getTileSizeY() const { return tileSize.y; }


	void setXPos(int a)  { yPos = a; }

	void setYPos(int a)  { yPos = a; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible) ;
	int amITr(const glm::ivec2 &pos);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible) ;
	bool collisionMoveUp(glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible) ;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible) ;
	double collisionMoveDownBallX(const glm::ivec2 &pos, const glm::ivec2 &posAnt, const glm::ivec2 &size, int b, int xSpeed, int ySpeed);
	double collisionMoveDownBallY(const glm::ivec2 &pos, const glm::ivec2 &posAnt,const glm::ivec2 &size, int b, int xSpeed, int ySpeed);
	bool collisionMoveDownPaddle(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2 &paddlePos) const;
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

	int getPoints() { return points;}

	int getHouse() { return house; }

	int getLives() { return lives; }

	int getHoney() { return honey; }

	int getKey() { return key; }
	
private:
	bool loadLevel(const string &levelFile);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int xPos;
	int yPos;
	int xPosAnt;
	int yPosAnt;
	glm::vec2 blockSize;
	glm::vec2 tileSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int offset = 3, offsetR = 93;
	int levelTile;
	int snitch;
	bool key;
	int points = 0;
	int house = 1;
	int lives = 4;
	int honey = 0;
	bool open1 = false;
	bool open2 = false;
	bool open3 = false;
};


#endif // _TILE_MAP_INCLUDE


