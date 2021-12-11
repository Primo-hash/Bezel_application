#pragma once

#include <engine/engine.h>
#include <fstream>
#include <string>
#include <vector>
#include <random>

#include <pacman/include/logic/collision.h>
#include "pacman/include/characters/pacman.h"
#include "pacman/include/characters/ghost.h"
#include "pacman/include/inanimate-objects/wall.h"
#include "pacman/include/inanimate-objects/pellet.h"

//Expected Matrix size
const int ROWMAX = 28, COLUMNMAX = 36;

// -----------------------------------------------------------------------------
// RANDOM NUMBER GENERATION
// -----------------------------------------------------------------------------
// ask OS for seed for random number gen
std::random_device rd;
std::mt19937 rando(rd());
// restricting number generation between 1 and 30
std::uniform_int_distribution<int> numDistribution(1, 40);	// Random number 40 is great number
std::uniform_int_distribution<int> dirDistribution(1, 4);	// Direction

class Map {
public:
	Map();

	void load();
	void onUpdate(engine::Time ts);
	void onRender();

	void gameOver();
	void reset();	// Run when game is over and players/ ghost get back to positions

	bool isGameOver() const { return m_GameOver; }
	int getRow() { return m_Row; }	// Accessor method for the row field
	int getColumn() { return m_Column; }	// Accessor method for the column field
	void* getMapMatrix() { return m_MapMatrix; }	// Accessor method for the mapMatrix

private:
	int m_MapMatrix[ROWMAX][COLUMNMAX];	//2D list that holds all values for the level
	int m_Row, m_Column;					//Actual RowSize and ColumnSize
	int m_Score = 0;

	bool m_GameOver = false;

	engine::ObjectLibrary* s_ObjectLibrary;
	engine::ShaderLibrary* s_ShaderLibrary;

	engine::s_Ptr<Pacman> m_Player;
	std::vector<engine::s_Ptr<Ghost>> m_Ghosts;	//Initialising an array of ghosts

	std::vector<engine::s_Ptr<Wall>> m_Walls;
	std::vector<engine::s_Ptr<Pellet>> m_Pellets;

	engine::s_Ptr <Collision> m_Collision = engine::m_SPtr<Collision>();
};

Map::Map() {
	load();

	s_ObjectLibrary = engine::Renderer::getObjectLibrary();
	s_ShaderLibrary = engine::Renderer::getShaderLibrary();

	// Load map objects
	engine::Renderer::loadShape("./assets/models/wall", "wall");
	engine::Renderer::loadShape("./assets/models/pellet", "pellet");
	engine::Renderer::loadShape("./assets/models/ghost", "ghost");
	engine::Renderer::loadShape("./assets/models/pac", "pac");
	
	//s_ObjectLibrary->loadObjectFromFile("wall", "./assets/models/wall/wall.obj");
	//s_ObjectLibrary->loadObjectFromFile("pellet", "./assets/models/pellet/pellet.obj");
	//s_ObjectLibrary->loadObjectFromFile("ghost", "./assets/models/ghost/ghost.obj");
	//s_ObjectLibrary->loadObjectFromFile("pac", "./assets/models/pac/pac.obj");

	// Load shaders
	//auto lightingShader = s_ShaderLibrary->load("assets/shaders/lighting-shader.glsl");


	auto& window = engine::AppFrame::get().getWindow();	// Setup view with camera
	// wall and vector will use predefined textures by engine

	int randomNumber;

	//Offset of each object we want to load in the axis we want to render
	float offsetX = 0.f;
	float offsetY = 0.f;

	// Position from camera
	glm::vec3 cam = { 0, 0, 0 };

	// Assign all positions per ID
	for (int i = 0; i < ROWMAX; i++) {
		offsetY = 0;
		offsetX += 0.f;
		for (int j = 0; j < COLUMNMAX; j++) {
			int value = m_MapMatrix[i][j];
			offsetY += 0.f;
			switch (value) {
			case 1:		// Wall
			{
				m_Walls.push_back(engine::m_SPtr<Wall>(i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z, 0.5f, 0.5f, 0.5f));
				break;
			}
			case 2:		// Player/Pacman
			{
				m_Player = engine::m_SPtr<Pacman>();
				m_Player->setPosition({ i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z });
				m_Player->setNextPosition({ i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z });
				m_Player->setSize({ 1.f, 1.f, 1.f });

				break;
			}
			case 0:	// Pellets and Ghosts
				m_Pellets.push_back(engine::m_SPtr<Pellet>(i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z, 0.1f, 0.1f, 0.1f));
	
				// Ghost random position generation
				randomNumber = numDistribution(rando);
	
				// Generate new random number for random ghost placement
				if (randomNumber == 3 && m_Ghosts.size() < 4) {
					m_Ghosts.push_back(engine::m_SPtr<Ghost>());
					m_Ghosts.back()->setPosition({ i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z });
					m_Ghosts.back()->setNextPosition({ i + offsetX + cam.x, j + offsetY + cam.y, 0.f + cam.z });
					m_Ghosts.back()->setSize({ 1.f, 1.f, 1.f });
				}
				break;
			default:
				APP_INFO("Unknown ID's on map, was not read");
				break;
			}
		}
	}

}

//Loading each object on the map
void Map::load() {
	//Find file that holds the map
	std::fstream file("assets/levels/level0.txt", std::ios_base::in);
	//If found, continue
	if (file) {
		//Read first line of the level - "28x36"
		file >> m_Row;				//Get row size - 28
		file.ignore(1, 'x');		//Ignore the first x in the document
		file >> m_Column;				//Get column size - 36

		//Iterate over the rest of the document line by line
		for (int i = 0; i < m_Column; i++) {		//For all columnValues
			for (int j = 0; j < m_Row; j++) {	//For each rowValues
				file.ignore();					//Ignore whitespace
				file >> m_MapMatrix[j][i];
			}
		}
	}
}

void Map::onUpdate(engine::Time ts) {
	m_Player->onUpdate(ts);
	
	//Check for collisions between walls and pacman
	for (auto it : m_Walls) {
		//Check if the position in the next frame will collide with a wall
		if (m_Collision->squareSquare(m_Player->getNextPosition(), m_Player->getSize(),
			it->getPosition(), it->getSize()))
		{
			//If it collides, set the next position to be the old position
			m_Player->setNextPosition(m_Player->getPosition());
			break;
		}

	}

	//Pacmans position is now updated
	m_Player->setPosition(m_Player->getNextPosition());

	//Check for collisions between pacman and ghosts
	for (auto it : m_Pellets) {
		if (m_Collision->squareSquare(m_Player->getNextPosition(), m_Player->getSize(),
			it->getPosition(), it->getSize()))
		{
			//If it has not already been ate
			if (!it->getIsEaten())
			{
				it->setEaten();
				m_Score++;
			}
		}
	}

	//Updating ghosts
	for (auto ghost : m_Ghosts) {
		ghost->onUpdate(ts);
		//Check if they have collided with any walls
		for (auto wall : m_Walls) {
			bool collided = m_Collision->squareSquare(ghost->getNextPosition(), ghost->getSize(),
				wall->getPosition(), wall->getSize());
			if (collided)
			{
				//If the ghost collides, we set the next position to be the old one
				ghost->setNextPosition(ghost->getPosition());
				//Find a new direction for the ghost
				ghost->setRandomDirection(dirDistribution(rando));
				break;
			}
		}
		//The iterated ghosts position is now updated
		ghost->setPosition(ghost->getNextPosition());
		//Check if Ghost collides with Pacman
		if (m_Collision->circleCircle(m_Player->getPosition(), m_Player->getRadius(),
			ghost->getPosition(), ghost->getRadius()))

		{
			gameOver();
		}
	}



}

//Renders the objects on the screen, objects declared first will be rendered last etc
void Map::onRender() {
	
	//Draw ghosts
	for (auto it : m_Ghosts) {
		it->onRender();
	}
	
	//Draw walls
	for (auto it : m_Walls) {
		it->onRender();
	}

	//Draw pac
	m_Player->onRender();
	
	//Draw pellets
	for (auto it : m_Pellets) {
		it->onRender();
	}
	
	
}


/*
	Ends level on game over
*/
void Map::gameOver() {
	m_GameOver = true;
	APP_INFO(m_Score);
}

void Map::reset() {
	m_GameOver = false;
	//	m_Player->reset();
	//  m_Ghost->reset();
	//	m_Pellet->reset();  isEaten må settes til false
}
