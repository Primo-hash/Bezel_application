#pragma once
#include <engine/engine.h>

class SandboxLayer : public engine::Layer {
public:
	/*
		Layer constructor
		- Defines default aspect ratio of camera
	*/
	SandboxLayer::SandboxLayer();

	void SandboxLayer::onAttach() override;
	void SandboxLayer::onDetach() override;
	void SandboxLayer::onUpdate(engine::Time ts) override;
	void SandboxLayer::onEvent(engine::Event& e) override;

private:
	// Rendering API
	//engine::s_Ptr<engine::Renderer> s_Renderer;
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
};

/*
	Layer constructor
	- Defines default aspect ratio of camera
*/
SandboxLayer::SandboxLayer() : engine::Layer(), m_CameraController(1600.0f / 900.0f) {
	// Init rendering api
	engine::s_Ptr<engine::Renderer> s_Renderer = engine::m_SPtr<engine::Renderer>();


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
	m_CameraController.setZoomLevel(10.f);

	// Loading up example textures for renderer method
	m_EnemyJetTexture = engine::m_SPtr<engine::Texture>("assets/textures/enemy-fighterjet.png");
	m_RocketShipTexture = engine::m_SPtr<engine::Texture>("assets/textures/rocket-ship.png");
}

void SandboxLayer::onDetach() {

}

void SandboxLayer::onUpdate(engine::Time ts) {

	// Render API draw calls
	engine::Renderer::get().setClearColor({ 0.5546875f, 0.5546875f, 0.5546875f, 1 });
	engine::Renderer::get().clear();

	// Camera scene init with view projection data setup
	engine::Renderer::beginScene(m_CameraController.getCamera());

	// Testing Quad and texture drawing

	// Normal Quad with position, size, rotation and color, quad drawn below textures since depth negative
	// More details in renderer.h
	engine::Renderer::drawRotatedQuad({ -4.0f, 0.0f, -1.f }, { 1.f, 1.f }, -45.0f, m_RocketShipTexture, 10.f);
	engine::Renderer::drawRotatedQuad({ -10.0f, 0.0f, -2.f }, { 1.f, 1.f }, -90.0f, m_SquareColor1);
	engine::Renderer::drawCircle({ -5.0f, 0.0f, 0.0f }, { 1.f, 1.f }, m_SquareColor1);

	// Using a texture and repeats it 10 times within scale constriction
	engine::Renderer::drawQuad({ 5.0f, 0.0f, 1.0f }, { 5.0f, 5.0f }, m_EnemyJetTexture, 10.f);

	engine::Renderer::endScene();
}

void SandboxLayer::onEvent(engine::Event& e) {
	m_CameraController.onEvent(e);
}