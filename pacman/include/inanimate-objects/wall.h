#include <engine/engine.h>
#include "pacman/include/color.h"

class Wall {
public:
	Wall(float x, float y, float z, float width, float length, float height) {
		setPosition({ x , y, z });
		setSize({ width, length, height});
	};

	void setPosition(glm::vec3 newPosition) { position = newPosition; }
	void setSize(glm::vec3 newSize) { size = newSize; }
	void onRender();

	glm::vec3 getSize() { return size; }
	glm::vec3 getPosition() { return position; }
private:
	glm::vec3 position;
	glm::vec3 size;
	glm::vec4 colour = color::WallBlue;
};

void Wall::onRender(){
	engine::Renderer::draw3DObject({ position.x, position.y, position.z }, 
		{ size.x, size.y, size.z }, 
		{ 0, 0, 0 }, 
		colour,
		"./assets/models/wall",
		"wall");
	//engine::Renderer::drawQuad({ position.x, position.y }, { size.x, size.x }, colour);
}