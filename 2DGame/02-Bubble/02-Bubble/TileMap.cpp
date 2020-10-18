#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	xPos = 0;
	yPos = 0;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	//sstream >> tileSize.x >> tileSize.x >> blockSize;
	tileSize = glm::vec2(16, 8);
	blockSize = glm::vec2(16, 16);
	cout << tileSize.x << " " << tileSize.x;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize.x, minCoords.y + j * tileSize.y);
				texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x ); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x ); vertices.push_back(posTile.y + blockSize.y );
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int b)
{

	int x, x1, y0, y1;
	if (b == 2){
		xPos = pos.x;
		yPos = pos.y;
	}
	x = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;

	if (b == 1)
		if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (8 + yPos > pos.y))
			return true;

	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0) {
			if(b == 1 && map[y*mapSize.x + x] != 8)
				map[y*mapSize.x + x] = 0;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int b) 
{
	int i = pos.x;

	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}

	int x0, x, y0, y1;

	x0 = pos.x / tileSize.x;
	x = (pos.x + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;

	if (b == 1)
		if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (8 + yPos > pos.y))
			return true;

	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0){
			if (b == 1 && map[y*mapSize.x + x] != 8)
				map[y*mapSize.x + x] = 0;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int b) 
{

	int x0, x1, y, y1;
	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}
	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y) / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;

	if (b == 1)
		if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (8 + yPos > pos.y))
			return true;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0){
			if (b == 1 && map[y*mapSize.x + x] != 8)
				map[y*mapSize.x + x] = 0;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int b) 
{

	int x0, x1, y, y0;
	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}
	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	y0 = pos.y / tileSize.y;

	if (b == 1)
		if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (8 + yPos > pos.y))
			return true;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0) {
			if (b == 1 && map[y*mapSize.x + x] != 8)
				map[y*mapSize.x + x] = 0;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveDownPaddle(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec2 &paddlePos) const
{
	if ((paddlePos.y - 16) == pos.y)
		if ((pos.x >= paddlePos.x) && (pos.x <= paddlePos.x + 32))
			return true;
	int x0, x1, y;

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0) {
			return true;
		}
	}

	return false;
}
































