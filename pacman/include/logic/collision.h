#pragma once

#include <engine/engine.h>
//Static class for collision calculation for circle & ball and square & cube shapes
class Collision {

public:
	//Checks if two circles are overlapping
	virtual bool circleCircle(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2);
	//Checks if a circle and a square are overlapping
	virtual bool circleSquare(glm::vec3 pos1, float radius1, glm::vec3 pos2, glm::vec3 size2);
	//Checks if two squares are overlapping
	virtual bool squareSquare(glm::vec3 pos1, glm::vec3 size1, glm::vec3 pos2, glm::vec3 size2);
};

bool Collision::circleCircle(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2) {
	//Make a vector from First Circle to second circle
	glm::vec3 distanceVector = pos1 - pos2;
	//Find the distance between the two centres
	float distance = glm::length(distanceVector);
	//If the distance is lower than both of their radiuses, they are overlapping
	if (distance <= radius1 + radius2)
	{
		return true;
	}
	return false;
}

bool Collision::circleSquare(glm::vec3 pos1, float radius1, glm::vec3 pos2, glm::vec3 size2) {
	return false;
}

bool Collision::squareSquare(glm::vec3 pos1, glm::vec3 size1, glm::vec3 pos2, glm::vec3 size2) {
	//Check for collision in X, Y, Z axis
	bool collisionX = pos1.x + size1.x > pos2.x && pos2.x + size2.x > pos1.x;

	bool collisionY = pos1.y + size1.y > pos2.y && pos2.y + size2.y > pos1.y;

	bool collisionZ = pos1.z + size1.z > pos2.z && pos2.z + size2.z > pos1.z;

	if (collisionX && collisionY && collisionZ)
	{
		return true;
	}

	return false;
}
