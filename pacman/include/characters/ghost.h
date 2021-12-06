#pragma once

#include <engine/engine.h>
#include <stdlib.h>

class Ghost {
public:
	Ghost();

	virtual void loadAssets();

	virtual void onUpdate(engine::Time ts);

	virtual void onRender();

	void reset();

	void setDirection(glm::vec3 newDirection) { m_Direction = newDirection; }	void setRandomDirection(int rand);
	//Mutator method for size field
	void setSize(glm::vec3 newSize) { m_Size = newSize; }
	//Mutator method for position field
	void setPosition(glm::vec3 newPosition) { m_Position = newPosition; }
	//Accessor method for position field
	const glm::vec3& getPosition() { return m_Position; }
	//Mutator method for position field
	void setNextPosition(glm::vec3 newNextPosition) { m_NextPosition = newNextPosition; }
	//Accessor method for position field
	const glm::vec3& getNextPosition() { return m_NextPosition; }
	//Accessor method for velocity
	const float getVelocity() { return m_Velocity; }
	//Mutator method for velocity
	void setVelocity(int velocity) { m_Velocity = velocity; }
	//accessor methd for size
	glm::vec3 getSize() { return m_Size; }
	float getRadius() { return m_Radius; }

private:
	glm::vec3 m_NextPosition = { 0,0,0 };
	glm::vec3 m_Position = { 0,0,0 };		//Position of character
	glm::vec3 m_Rotation = { 0,0,1.f };		//Position of character
	glm::vec3 m_Direction = { 0.f , 0.f , 0.f };    //Directional vector
	glm::vec3 m_Size = { 0.8, 0.8, 0.8 };

	float m_Radius = 0.4f;				//Radius of ghosties
	float m_Velocity = 2.0f;			//Velocity of character
	float m_Time = 0.0f;				//Current time

	glm::vec4 m_Color = { 0.8f, 1.0f, 0.69f, 1.0f };

	// 0 = down, 1 = left, 2 = right, 3 = up
	int m_TextureDirection = 0;
	int m_DirectionNumber = 2;
	std::string cycles[4] = { "assets/textures/ghost-down.png",
							  "assets/textures/ghost-left.png",
							  "assets/textures/ghost-right.png",
							  "assets/textures/ghost-up.png" };

	engine::s_Ptr<engine::Texture[]> m_textures[4];
	std::vector<engine::s_Ptr<engine::Texture>> m_Textures;
};

inline Ghost::Ghost()
{
	loadAssets();
}

void Ghost::loadAssets()
{
	for (int i = 0; i < 4; i++) {
		m_Textures.push_back(engine::m_SPtr<engine::Texture>(cycles[i]));
	}
}

void Ghost::onUpdate(engine::Time ts)
{
	m_Time += ts;

	if (m_Time == 0)
	{
		setDirection({ 1,0,0 });
		ts = 0.001f;
	}
	float timestep = ts.getSeconds();

	//position vector = position + time * m/s * direction vector
	m_NextPosition.x += timestep * m_Velocity * m_Direction.x;
	m_NextPosition.y += timestep * m_Velocity * m_Direction.y;
	m_NextPosition.z += timestep * m_Velocity * m_Direction.z;

	m_Time += ts;

	// ghost kolliderer wall -> wall

}

void Ghost::onRender()
{
	engine::Renderer::draw3DObject({ m_NextPosition.x, m_NextPosition.y, m_NextPosition.z }, 
		{ m_Size.x, m_Size.y, m_Size.z }, 
		m_Rotation, m_Color, 
		"./assets/models/ghost",
		"ghost");
	//engine::Renderer::drawQuad({ m_NextPosition.x, m_NextPosition.y, 0.0f }, { m_Size.x, m_Size.y }, m_Textures[m_TextureDirection]);
}

inline void Ghost::setRandomDirection(int rand)
{
	switch (rand) {
	case 0:
		m_DirectionNumber = 0;
		setDirection({ 0, -1, 0 });
		break;
	case 1:
		m_DirectionNumber = 1;
		setDirection({ -1, 0, 0 });
		break;
	case 2:
		m_DirectionNumber = 2;
		setDirection({ 1, 0, 0 });
		break;
	case 3:
		m_DirectionNumber = 3;
		setDirection({ 0, 1, 0 });
		break;
	}
}
