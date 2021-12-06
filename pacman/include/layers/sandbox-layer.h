#pragma once

/*
#include <engine/engine.h>

#include "pacman/include/characters/pacman.h"
#include "pacman/include/inanimate-objects/map.h"

class SandboxLayer : public engine::Layer {
public:
	SandboxLayer();
	 
	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(engine::Time ts) override;
	void onEvent(engine::Event& e) override;
private:
	// Rendering API
	engine::u_Ptr<engine::Renderer> s_Renderer;
	engine::ShaderLibrary m_ShaderLibrary;			// For handling of shader program files

	// Camera
	engine::OrthoCameraController m_CameraController;

	// Generation of shaders using vertex array testing
	engine::s_Ptr<engine::VertexArray> m_SquareVertexArray;

	// Textures testing
	engine::s_Ptr<engine::Texture> m_EnemyJetTexture;
	engine::s_Ptr<engine::Texture> m_RocketShipTexture;

	glm::vec4 m_SquareColor1 = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareColor2 = { 0.2f, 0.3f, 0.8f, 1.0f };
	//Characters
	Pacman m_player;
};

/*
	Layer constructor
	- Defines default aspect ratio of camera

SandboxLayer::SandboxLayer() : Layer(), m_CameraController(1600.0f / 900.0f) {
	// Init rendering api
	engine::u_Ptr<engine::Renderer> s_Renderer = engine::m_UPtr<engine::Renderer>();

	// OPTIONS

	// Turn on blending for OpenGL
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	// When blending uses the inverse of src texture to determine drawing priority
	// In this case the destination texture is only drawn if RGBA values of src are 0
	// Allowing "transparency" in textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void SandboxLayer::onAttach() {
	// Loading up example textures for renderer method
	m_EnemyJetTexture = engine::m_SPtr<engine::Texture>("assets/textures/enemy-fighterjet.png");
	m_RocketShipTexture = engine::m_SPtr<engine::Texture>("assets/textures/rocket-ship.png");

	// Setting up a more elaborate drawing of colored squares using vertex array method
	m_SquareVertexArray = engine::m_SPtr<engine::VertexArray>();

	// coords, texcoords
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	// Vertex buffer setup with datatype definition and stride indication of squareVertices
	engine::s_Ptr<engine::VertexBuffer> squareVertexBuffer = engine::m_SPtr<engine::VertexBuffer>(squareVertices, sizeof(squareVertices));
	squareVertexBuffer->setLayout({
		{ engine::ShaderDataType::Float3, "a_Position" },
		{ engine::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVertexArray->setVertexBuffer(squareVertexBuffer);

	// Index buffer setup with datatype orientation on draw
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	engine::s_Ptr<engine::IndexBuffer> squareIndexBuffer = engine::m_SPtr <engine::IndexBuffer>(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->setIndexBuffer(squareIndexBuffer);

	// load shader programs from filesystem to shader library
	auto squareShader = m_ShaderLibrary.load("assets/shaders/square-color.glsl");
	squareShader->bind();											// Select shader
	squareShader->addUniformFloat3("u_Color", m_SquareColor2);		// Set color
}

void SandboxLayer::onDetach() {

}

void SandboxLayer::onUpdate(engine::Time ts) {
	// Update camera position based on time (position is currently constant)
	//m_CameraController.onUpdate(ts);
	// Render API draw calls
	s_Renderer->get().setClearColor({ 0.5546875f, 0.5546875f, 0.5546875f, 1 });
	s_Renderer->get().clear();

	// Camera scene init with view projection data setup
	s_Renderer->beginScene(m_CameraController.getCamera());
	m_CameraController.setZoomLevel(10);
	// Testing Quad and texture drawing
	m_player.onRender();
	// Normal Quad with position, size, rotation and color, quad drawn below textures since depth negative
	// More details in renderer.h
	s_Renderer->drawRotatedQuad({ -1.0f, 0.0f, -0.1f }, { 10.f, 10.f }, glm::radians(-45.0f), m_SquareColor1);

	// Using a texture and repeats it 10 times within scale constriction
	s_Renderer->drawQuad({ 0.0f, -5.0f, 0.0f }, { 5.0f, 5.0f }, m_EnemyJetTexture, 10.f);

	s_Renderer->drawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_RocketShipTexture);

	// Testing custom shader program drawing
	glm::vec3 pos(-3.f, 0.f, 0.f);											// Changes x and y position for each square
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));			// scales of square
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

	auto squareShader = m_ShaderLibrary.get("square-color");
	s_Renderer->submit(squareShader, m_SquareVertexArray, transform);		// Submit square
	s_Renderer->endScene();
}

void SandboxLayer::onEvent(engine::Event& e) {
	m_CameraController.onEvent(e);
}
*/