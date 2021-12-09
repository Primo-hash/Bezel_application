#pragma once
#include <engine/engine.h>

#include "pacman/include/inanimate-objects/map.h"

class GameLayer : public engine::Layer {
public:
	GameLayer();

	void onAttach() override;
	void onDetach() override;

	void onUpdate(engine::Time ts) override;
	void onEvent(engine::Event& e) override;
	void setCamera(uint32_t width, uint32_t height);

	bool onWindowResized(engine::WindowResizeEvent& e);
	bool onWindowClosed(engine::WindowCloseEvent& e);
private:
	enum State {	// State of the game 
		InGame, GameOver
	};

	State m_State = State::InGame;
	float m_Time = 0.0f;				// Time elapsed since game start

	engine::u_Ptr<Map> m_Map;

	// Camera
	engine::CameraController m_CameraController;
	//engine::u_Ptr<engine::OrthographicCamera> m_Camera;
};


// ====================
/*
	Layer constructor
	- Defines default aspect ratio of camera
*/
GameLayer::GameLayer() : engine::Layer(), m_CameraController(1600.0f/900.0f, 90.f, true) {
	m_Map = engine::m_UPtr<Map>();

	auto& window = engine::AppFrame::get().getWindow();	// Setup view with camera
	//setCamera(window.getWidth(), window.getHeight());
	//m_CameraController.getCamera().setPosition({ m_Map->getRow() / 2.f, m_Map->getColumn() / 2.f, 0 });
	m_CameraController.setZoomLevel(m_Map->getColumn() / 3.f);

	// Init rendering api has to be done for rendering components to initialize
	engine::u_Ptr<engine::Renderer> s_Renderer = engine::m_UPtr<engine::Renderer>();

	// OPTIONS

	// CULLING
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Turn on blending for OpenGL
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// When blending uses the inverse of src texture to determine drawing priority
	// In this case the destination texture is only drawn if RGBA values of src are 0
	// Allowing "transparency" in textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Focus mouse on window when it is active and tie to middle of screen
	glfwSetInputMode(static_cast<GLFWwindow*>(window.getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void GameLayer::onAttach() {

}

void GameLayer::onDetach() {

}

/*
	Game loop
*/
void GameLayer::onUpdate(engine::Time ts) {
	m_Time += ts;

	//auto& window = engine::AppFrame::get().getWindow();	// Setup view with camera
	//setCamera(window.getWidth(), window.getHeight());
	//m_Camera->setPosition({ m_Map->getRow()/2.f, m_Map->getColumn() / 2.f, 0 });
	m_CameraController.onUpdate(ts);

	// Checks if game is over
	if (m_Map->isGameOver()) {										// Check for game over from level
		m_State = State::GameOver;									// Set layer state to game over
	}

	// Only updates entities if game in play
	switch (m_State) {		// Game states
		case State::InGame: {							// Game still in play
			m_Map->onUpdate(ts);	// Run game loop protocol for level
			break;
		}
	}


	// Render API draw calls
	engine::Renderer::get().setClearColor({ 0.0f, 0.0f,0.0f, 1.f });
	//engine::Renderer::get().clear(); Now run in engine

	// Camera scene init with view projection data setup
	engine::Renderer::beginScene(m_CameraController.getCamera());
	
	m_Map->onRender();
	
	engine::Renderer::endScene();
}

void GameLayer::onEvent(engine::Event& e) {
	engine::EventDispatcher dispatcher(e);
	//dispatcher.Dispatch<engine::WindowResizeEvent>(BIND_EVENT_FN(GameLayer::onWindowResized));
	m_CameraController.onEvent(e);
}

void GameLayer::setCamera(uint32_t width, uint32_t height) {
	float aspectRatio = (float)width / (float)height;
	float zoom = m_Map->getColumn() / 2;
	float bottom = -zoom;
	float top = zoom - 0.5f;
	float left = bottom * aspectRatio + 3.5f;
	float right = top * aspectRatio - 4.f;

	//m_Camera = engine::m_UPtr<engine::OrthographicCamera>(left, right, bottom, top);
	//m_Camera->setPosition({ width/10/2-0.5f, height/10/2-0.5f, 0 });
}

bool GameLayer::onWindowResized(engine::WindowResizeEvent& e) {
	setCamera(e.getWidth(), e.getHeight());
	return false;
}

