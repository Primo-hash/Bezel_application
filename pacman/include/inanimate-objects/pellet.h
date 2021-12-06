#include <engine/engine.h>
#include "pacman/include/color.h"

class Pellet {
public:
	Pellet(float x, float y, float z, float width, float length, float height) {
		setPosition({ x , y, z });
		setSize({ width, length, height });
	};

	void setPosition(glm::vec3 newPosition) { position = newPosition; }
	void setSize(glm::vec3 newSize) { size = newSize; }
	void onRender();
	void setEaten() { isEaten = true; }
	bool getIsEaten() { return isEaten; }
	glm::vec3 getPosition() { return position; }
	glm::vec3 getSize() { return size; }

private:
	bool isEaten = false;
	glm::vec3 position;
	glm::vec3 size;
	glm::vec4 colour = color::PelletYellow;
};

void Pellet::onRender() {
	if (isEaten == false)
	{
		engine::Renderer::draw3DObject({ position.x, position.y, position.z },
			{ size.x, size.y, size.z },
			{ 0, 0, 0 },
			colour,
			"./assets/models/pellet",
			"pellet");
		//engine::Renderer::drawCircle({ position.x, position.y }, { size.x, size.y }, colour);
	}
	else { 
		return; 
	}
}