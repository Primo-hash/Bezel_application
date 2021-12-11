#pragma once

#include <engine/engine.h>
class Pacman
{
public:
	Pacman();

	virtual void loadAssets();

	virtual void onUpdate(engine::Time ts);

	virtual void onRender();

	void reset();

	void setSize(glm::vec3 newSize) { m_Size = newSize; }
	void setDirection(glm::vec3 newDirection) { m_direction = newDirection; }
	//Mutator method for position field
	void setPosition(glm::vec3 newPosition) { m_position = newPosition; }
	//Accessor method for position field
	const glm::vec3& getPosition() { return m_position; }
	//Mutator method for position field
	void setNextPosition(glm::vec3 newNextPosition) { m_NextPosition = newNextPosition; }
	//Accessor method for position field
	const glm::vec3& getNextPosition() { return m_NextPosition; }
	//Accessor method for velocity
	const float getVelocity() { return m_velocity; }
	//Mutator method for velocity
	void setVelocity(float velocity) { m_velocity = velocity; }
	//Accessor method for rotation
	float getRotation() { return m_rotation; }
	//get Radius
	float getRadius() { return m_radius; }
	//accessor methd for size
	glm::vec3 getSize() { return m_Size; }
private:
	float m_radius = 1.f;         //Radius of pacyman
	float m_velocity = 2.0f;	   //Velocity of character
	float m_time = 0.0f;		   //Current time

	float m_rotation = 0.0f; //Rotation of the pacman
	glm::vec3 m_Rotation = { 0.f, 0.f, 0.f };		//Rotation of character in 3d space

	glm::vec3 m_NextPosition = { 0,0,0 };
	glm::vec3 m_position = { 0,0,0 };	   //Position of character
	glm::vec3 m_direction = {0,0,0};    //Directional vector
	
	glm::vec3 m_Size = { m_radius, m_radius, m_radius };

	//Variables for pacman cycles
	float m_lastCycle = 0.0f;
	const float m_cycleTimer = .05f;
	const int assetNumber = 4;
	int cycleNumber = 0;
	int direction = 1; 
	std::string cycles[4] = { "assets/textures/pacman0.png",
							  "assets/textures/pacman1.png",
							  "assets/textures/pacman2.png",
							  "assets/textures/pacman3.png" };

	engine::s_Ptr<engine::Texture[]> m_textures[4];
	std::vector<engine::s_Ptr<engine::Texture>> m_Textures;
};

glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t) {
	return x * (1.f - t) + y * t;
}

Pacman::Pacman()
{
	loadAssets(); 
}

void Pacman::loadAssets()
{
	//m_texture0 = engine::m_SPtr<engine::Texture>("assets/textures/pacman0.png");
	for(int i = 0; i < 4; i++) {
		m_Textures.push_back(engine::m_SPtr<engine::Texture>(cycles[i]));
	}
}

//Ts = time since last frame??
void Pacman::onUpdate(engine::Time ts) 
{
	m_time += ts;
	//Check if pacman needs to change cycle
	m_lastCycle += ts;
	//If lastCycle is a higher number than cycleTimer
	if (m_lastCycle > m_cycleTimer)     //Check if pacman is due for a cycle change
	{
		cycleNumber += direction;				//Advance in cycle
		if (cycleNumber > 2)			//If higher than the number of assets, go back to 0 
			{
			direction = -1;				//Go backwards in the animation
		}
		else if (cycleNumber < 1) {			//
			direction = 1;				//Go forwards in the animation
		}
		m_lastCycle = 0.0f;
	}
	//position vector = position + time * m/s * direction vector
	m_NextPosition.x += ts.getSeconds() * m_velocity * m_direction.x;
	m_NextPosition.y += ts.getSeconds() * m_velocity * m_direction.y;
	m_NextPosition.z += ts.getSeconds() * m_velocity * m_direction.z;

	//Check if there is input for new direction
	//W - Upwards
	if (engine::Input::isKeyPressed(GLFW_KEY_UP)) {
		setDirection({ 0, 1, 0 });
		m_rotation = 90.0f;
	}
	//S - Downwards
	if (engine::Input::isKeyPressed(GLFW_KEY_DOWN)) {
		setDirection({ 0, -1, 0 });
		m_rotation = -90.0f;
	}
	//D - right
	if (engine::Input::isKeyPressed(GLFW_KEY_RIGHT)) {
		setDirection({ 1, 0, 0 });
		m_rotation = 0.0f;
	}
	//A - left
	if (engine::Input::isKeyPressed(GLFW_KEY_LEFT)) {
		setDirection({ -1, 0, 0 });
		m_rotation = 180.0f;
	}
}

void Pacman::onRender() 
{
	engine::Renderer::draw3DObject(m_NextPosition,
		m_Size,
		m_Rotation, {0, 1, 0, 1},
		"./assets/models/pac",
		"pac");
	//engine::Renderer::drawRotatedQuad({ m_NextPosition.x, m_NextPosition.y, 0.0f}, { m_Size.x, m_Size.y }, m_rotation, m_Textures[cycleNumber]);
}

