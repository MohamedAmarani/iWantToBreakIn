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

	void render() const;
	void free();
	
	int getTileSizeX() const { return tileSize.x; }

	int getTileSizeY() const { return tileSize.y; }


	void setXPos(int a)  { yPos = a; }

	void setYPos(int a)  { yPos = a; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int b) ;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int b) ;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int b) ;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int b) ;
	int collisionMoveDownBall(const glm::ivec2 &pos, const glm::ivec2 &size, int b);
	bool collisionMoveDownPaddle(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2 &paddlePos) const;
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

	
private:
	bool loadLevel(const string &levelFile);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int xPos;
	int yPos;
	glm::vec2 blockSize;
	glm::vec2 tileSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int offset, offsetR;
	int levelTile;

};


#endif // _TILE_MAP_INCLUDE


