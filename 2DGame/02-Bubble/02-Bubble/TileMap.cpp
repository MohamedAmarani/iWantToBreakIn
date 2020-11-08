#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "cmath"
#include "Game.h"

using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	offset = 3;
	key = false;
	xPos = 0;
	yPos = 0;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

int TileMap::getOffset() {
	return offset;
}

void TileMap::setOffset(int off) {
	offset = off;
}

int TileMap::getOffseR() {
	return offsetR;
}

void TileMap::render() const
{
	glTranslatef(1300, 0.0, 0.0);
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
	char tile, tile1;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	//mapSize.x = 13;
	//mapSize.y = 65;
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	//sstream >> tileSize.x >> tileSize.x >> blockSize;
	tileSize = glm::vec2(32, 16);
	blockSize = glm::vec2(32, 32);
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

	levelTile = 28;

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin.get(tile);
			fin.get(tile1);
			map[j*mapSize.x + i] = (tile - int('0')) * 10 + (tile1 - int('0'));
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

	if (offset == 0 && offsetR > 0)
		--offsetR;
	else if (offset == 0 && offsetR < 0)
		++offsetR;
	if (offset == 1 && offsetR > 31)
		--offsetR;
	else if (offset == 1 && offsetR < 31)
		++offsetR;
	if (offset == 2 && offsetR > 62)
		--offsetR;
	else if (offset == 2 && offsetR < 62)
		++offsetR;
	if (offset == 3 && offsetR > 93)
		--offsetR;
	else if (offset == 3 && offsetR < 93)
		++offsetR;

	for (int j = offsetR; j<levelTile + offsetR; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile 28x14
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize.x, minCoords.y + (j - offsetR) * tileSize.y);

				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
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
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));

}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

int TileMap::amITr(const glm::ivec2 &pos) {
	if (pos.y < 15 && offset>0) {
		++house;
		--offset;
		key = false;
		return 0;
	}
	else if (pos.y > 26 * 16 && offset == 3) {
		--lives;
		return 4;
	}
	else if (pos.y > 27 * 16 && offset < 3) {
		--house;
		++offset;
		return 1;
	}

	if (offset == 0 && offsetR == 0)
		return 2;
	else if (offset == 1 && offsetR == 31)
		return 2;
	else if (offset == 2 && offsetR == 62) {
		return 2;
	}
	else if (offset == 3 && offsetR == 93)
		return 2;

	return 3;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible)
{

	int x, x1, y0, y1;
	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}

	//checkear collision de pelota con winnie
	if (!chase && offset == 0 && b == 1 && (pos.x < 192 + 28) && (pos.x + 16 > 192) && (pos.y < 274 + 16) && (16 + pos.y > 274)) {
		chase = true;
		return true;
	}
	//checkear collision de pelota con llave
	if (!visible && b == 1 && offset == 1 && (pos.x + 16 > 84) && (pos.x < 84 + 24) && (pos.y + 16 > 243) && (pos.y < 243 + 24)) {
		key = true;
		if (!open3) {
			Game::instance().playSound("sounds/open.mp3");
			open3 = true;
		}
		return true;
	}
	if (!visible && b == 1 && offset == 2 && (pos.x + 16 > 230) && (pos.x < 230 + 24) && (pos.y + 16 > 196) && (pos.y < 196 + 24)) {
		key = true;
		if (!open2) {
			Game::instance().playSound("sounds/open.mp3");
			open2 = true;
		}
		return true;
	}
	if (!visible && b == 1 && offset == 3 && (pos.x + 16 > 231) && (pos.x < 231 + 24) && (pos.y + 16 > 101) && (pos.y < 101 + 24)) {
		key = true;
		if (!open1) {
			Game::instance().playSound("sounds/open.mp3");
			open1 = true;
		}
		return true;
	}

	x = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y0 += offset * 3;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	y1 += offset * 3;
	int u;
	for (int y = y0; y <= y1; y++)
	{
		if (key && offset != 0) {
			map[(3 * offset + (offset * levelTile))*mapSize.x + 5] = 0;
			map[(3 * offset + (offset * levelTile))*mapSize.x + 6] = 0;
			map[(3 * offset + (offset * levelTile))*mapSize.x + 7] = 0;
			map[(3 * offset + (offset * levelTile))*mapSize.x + 8] = 0;
		}
		if (map[(y + (offset * levelTile))*mapSize.x + x] != 0) {
			if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				Game::instance().playSound("sounds/choc.wav");
			}
			if (b != 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				return true;
			}
			if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 13 && map[(y + (offset * levelTile))*mapSize.x + x] != 14 && map[(y + (offset * levelTile))*mapSize.x + x] != 15
				&& map[(y + (offset * levelTile))*mapSize.x + x] != 16) {
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3) {
					if (map[(y + (offset * levelTile))*mapSize.x + x] > 16)
						Game::instance().playSound("sounds/miel.wav");
					else
						Game::instance().playSound("sounds/block.mp3");
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 17) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 18) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 - 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y - 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 8 && map[(y + (offset * levelTile))*mapSize.x + x] != 10
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 11 && map[(y + (offset * levelTile))*mapSize.x + x] != 1
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 17 && map[(y + (offset * levelTile))*mapSize.x + x] != 18) {

					if (map[(y + (offset * levelTile))*mapSize.x + x] < 13)
						points += 100;

					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 8 || map[(y + (offset * levelTile))*mapSize.x + x] == 10
					|| map[(y + (offset * levelTile))*mapSize.x + x] == 11))
					++map[(y + (offset * levelTile))*mapSize.x + x];
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible)
{
	int i = pos.x;

	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}

	//checkear collision de pelota con winnie
	if (!chase && offset == 0 && b == 1 && (pos.x < 192 + 28) && (pos.x + 16 > 192) && (pos.y < 274 + 16) && (16 + pos.y > 274)) {
		chase = true;
		return true;
	}
	//checkear collision de pelota con llave
	if (!visible && b == 1 && offset == 1 && (pos.x + 16 > 84) && (pos.x < 84 + 24) && (pos.y + 16 > 243) && (pos.y < 243 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 2 && (pos.x + 16 > 230) && (pos.x < 230 + 24) && (pos.y + 16 > 196) && (pos.y < 196 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 3 && (pos.x + 16 > 231) && (pos.x < 231 + 24) && (pos.y + 16 > 101) && (pos.y < 101 + 24)) {
		key = true;
		return true;
	}

	int x0, x, y0, y1;

	x0 = pos.x / tileSize.x;
	x = (pos.x + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y0 += offset * 3;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	y1 += offset * 3;
	for (int y = y0; y <= y1; y++)
	{
		if (map[(y + (offset * levelTile))*mapSize.x + x] != 0) {
			if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				Game::instance().playSound("sounds/choc.wav");
			}
			if (b != 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				return true;
			}
			if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 13 && map[(y + (offset * levelTile))*mapSize.x + x] != 14 && map[(y + (offset * levelTile))*mapSize.x + x] != 15
				&& map[(y + (offset * levelTile))*mapSize.x + x] != 16) {
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3) {
					if (map[(y + (offset * levelTile))*mapSize.x + x] > 16)
						Game::instance().playSound("sounds/miel.wav");
					else
						Game::instance().playSound("sounds/block.mp3");
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 17) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 18) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 - 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y - 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 8 && map[(y + (offset * levelTile))*mapSize.x + x] != 10
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 11 && map[(y + (offset * levelTile))*mapSize.x + x] != 1
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 17 && map[(y + (offset * levelTile))*mapSize.x + x] != 18) {

					if (map[(y + (offset * levelTile))*mapSize.x + x] < 13)
						points += 100;

					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 8 || map[(y + (offset * levelTile))*mapSize.x + x] == 10
					|| map[(y + (offset * levelTile))*mapSize.x + x] == 11))
					++map[(y + (offset * levelTile))*mapSize.x + x];
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionMoveUp(glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible)
{

	int x0, x1, y, y1;
	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}

	//checkear collision de pelota con winnie
	if (!chase && offset == 0 && b == 1 && (pos.x < 192 + 28) && (pos.x + 16 > 192) && (pos.y < 274 + 16) && (16 + pos.y > 274)) {
		chase = true;
		return true;
	}
	//checkear collision de pelota con llave
	if (!visible && b == 1 && offset == 1 && (pos.x + 16 > 84) && (pos.x < 84 + 24) && (pos.y + 16 > 243) && (pos.y < 243 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 2 && (pos.x + 16 > 230) && (pos.x < 230 + 24) && (pos.y + 16 > 196) && (pos.y < 196 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 3 && (pos.x + 16 > 231) && (pos.x < 231 + 24) && (pos.y + 16 > 101) && (pos.y < 101 + 24)) {
		key = true;
		return true;
	}

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y) / tileSize.y;
	y += offset * 3;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	y1 += offset * 3;
	int u;
	for (int x = x0; x <= x1; x++)
	{
		if (map[(y + (offset * levelTile))*mapSize.x + x] != 0) {
			if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				Game::instance().playSound("sounds/choc.wav");
			}
			if (b != 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				return true;
			}
			if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 13 && map[(y + (offset * levelTile))*mapSize.x + x] != 14 && map[(y + (offset * levelTile))*mapSize.x + x] != 15
				&& map[(y + (offset * levelTile))*mapSize.x + x] != 16) {
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3) {
					if (map[(y + (offset * levelTile))*mapSize.x + x] > 16)
						Game::instance().playSound("sounds/miel.wav");
					else
						Game::instance().playSound("sounds/block.mp3");
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 17) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 18) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 - 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y - 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 8 && map[(y + (offset * levelTile))*mapSize.x + x] != 10
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 11 && map[(y + (offset * levelTile))*mapSize.x + x] != 1
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 17 && map[(y + (offset * levelTile))*mapSize.x + x] != 18) {

					if (map[(y + (offset * levelTile))*mapSize.x + x] < 13)
						points += 100;

					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 8 || map[(y + (offset * levelTile))*mapSize.x + x] == 10
					|| map[(y + (offset * levelTile))*mapSize.x + x] == 11))
					++map[(y + (offset * levelTile))*mapSize.x + x];
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int b, bool visible)
{

	int x0, x1, y, y0;
	if (b == 2) {
		xPos = pos.x;
		yPos = pos.y;
	}

	//checkear collision de pelota con winnie
	if (!chase && offset == 0 && b == 1 && (pos.x < 192 + 28) && (pos.x + 16 > 192) && (pos.y < 274 + 16) && (16 + pos.y > 274)) {
		chase = true;
		return true;
	}
	//checkear collision de pelota con llave
	if (!visible && b == 1 && offset == 1 && (pos.x + 16 > 84) && (pos.x < 84 + 24) && (pos.y + 16 > 243) && (pos.y < 243 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 2 && (pos.x + 16 > 230) && (pos.x < 230 + 24) && (pos.y + 16 > 196) && (pos.y < 196 + 24)) {
		key = true;
		return true;
	}
	if (!visible && b == 1 && offset == 3 && (pos.x + 16 > 231) && (pos.x < 231 + 24) && (pos.y + 16 > 101) && (pos.y < 101 + 24)) {
		key = true;
		return true;
	}

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	y += offset * 3;
	y0 = pos.y / tileSize.y;
	y0 += offset * 3;
	int u;
	for (int x = x0; x <= x1; x++)
	{
		if (map[(y + (offset * levelTile))*mapSize.x + x] != 0) {
			if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				Game::instance().playSound("sounds/choc.wav");
			}
			if (b != 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 1 || map[(y + (offset * levelTile))*mapSize.x + x] == 2 || map[(y + (offset * levelTile))*mapSize.x + x] == 3)) {
				return true;
			}
			if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 13 && map[(y + (offset * levelTile))*mapSize.x + x] != 14 && map[(y + (offset * levelTile))*mapSize.x + x] != 15
				&& map[(y + (offset * levelTile))*mapSize.x + x] != 16) {
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3) {
					if (map[(y + (offset * levelTile))*mapSize.x + x] > 16)
						Game::instance().playSound("sounds/miel.wav");
					else
						Game::instance().playSound("sounds/block.mp3");
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 17) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] == 18) {
					honey += 100;
					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
					u = 0;
					u = x % 2;
					if ((y + offset + 1 - 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y - 1 + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && map[(y + (offset * levelTile))*mapSize.x + x] != 8 && map[(y + (offset * levelTile))*mapSize.x + x] != 10
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 11 && map[(y + (offset * levelTile))*mapSize.x + x] != 1
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 2 && map[(y + (offset * levelTile))*mapSize.x + x] != 3
					&& map[(y + (offset * levelTile))*mapSize.x + x] != 17 && map[(y + (offset * levelTile))*mapSize.x + x] != 18) {

					if (map[(y + (offset * levelTile))*mapSize.x + x] < 13)
						points += 100;

					int u = x % 2;
					if ((y + offset + 1) % 2 == 0)
						u += 2;
					else
						u += 0;
					map[(y + (offset * levelTile))*mapSize.x + x] = 13 + u;
				}
				if (b == 1 && (map[(y + (offset * levelTile))*mapSize.x + x] == 8 || map[(y + (offset * levelTile))*mapSize.x + x] == 10
					|| map[(y + (offset * levelTile))*mapSize.x + x] == 11))
					++map[(y + (offset * levelTile))*mapSize.x + x];
				return true;
			}
		}
	}

	return false;
}

double TileMap::collisionMoveDownBallX(const glm::ivec2 &pos, const glm::ivec2 &posAnt, const glm::ivec2 &size, int b, int xSpeed, int ySpeed)
{

	int x0, x1, y, y0;

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	y += offset;
	y0 = pos.y / tileSize.y;
	y0 += offset;

	double ballWidth = 16;
	double ballCenterX = pos.x + ballWidth / 2;
	double paddleWidth = 32;
	double paddleCenterX = xPos + paddleWidth / 2;
	double speedX = 4;
	double speedY = 4;

	double speedXY = sqrt(speedX*speedX + speedY*speedY);

	double posX = (ballCenterX - paddleCenterX) / ((paddleWidth + 15) / 2);
	double influenceX = 1;

	speedX = speedXY * posX * influenceX;

	speedY = sqrt((speedXY*speedXY - speedX*speedX >= 0) ? speedXY*speedXY - speedX*speedX : 0) * (speedY > 0 ? -1 : 1);

	if (b == 1)
		if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (yPos > pos.y) && ySpeed > 0) {
			Game::instance().playSound("sounds/paddle.wav");
			return speedX;
		}
	return -100;

}

double TileMap::collisionMoveDownBallY(const glm::ivec2 &pos, const glm::ivec2 &posAnt, const glm::ivec2 &size, int b, int xSpeed, int ySpeed)
{

	int x0, x1, y, y0;

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	y0 = pos.y / tileSize.y;

	double ballWidth = 16;
	double ballCenterX = pos.x + ballWidth / 2;
	double paddleWidth = 32;
	double paddleCenterX = xPos + paddleWidth / 2;
	double speedX = 4;
	double speedY = 4;

	double speedXY = sqrt(speedX*speedX + speedY*speedY);

	double posX = (ballCenterX - paddleCenterX) / ((paddleWidth + 15) / 2);
	double influenceX = 1;

	speedX = speedXY * posX * influenceX;

	speedY = sqrt((speedXY*speedXY - speedX*speedX >= 0) ? speedXY*speedXY - speedX*speedX : 0) * (speedY > 0 ? -1 : 1);

	if ((xPos < pos.x + size.x) && (xPos + 32 > pos.x) && (yPos < pos.y + size.y) && (yPos > pos.y) && ySpeed > 0)
		return speedY;
	else
		return -100;

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