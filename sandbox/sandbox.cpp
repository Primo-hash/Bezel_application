#include <engine/engine.h>
#include "include/layers/sandbox-layer.h"


/*
	An example Layer class

class ExampleLayer : public engine::Layer {
public:

	ExampleLayer() : Layer(), m_CameraController(1600.0f / 900.0f) {
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

	~ExampleLayer() {

	}

	/*
		Linear interpolation
	
	float lerp(float x0, float x1, float t) {
		return (1 - t) * x0 + t * x1;
	}

	void onUpdate(engine::Time ts) override {
		// Example of local polling of keyboard events from preinit Input instance
		if (engine::Input::isKeyPressed(GLFW_KEY_TAB)) {
			APP_TRACE("Tab key is pressed (poll)!");
		}

		s_Renderer->get().setClearColor({1, 1, 1, 0});
		s_Renderer->get().clear();


		// Testing custom shader program drawing
		glm::vec3 pos(-3.f, 0.f, 0.f);											// Changes x and y position for each square
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));			// scales of square
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

		auto squareShader = m_ShaderLibrary.get("square-color");
		s_Renderer->submit(squareShader, m_SquareVertexArray, transform);		// Submit square
	}

	void onEvent(engine::Event& event) override {
		// Example of application wide polling of keyboard events
		// To demonstrate, the application will poll all keyboard key presses
		if (event.getEventType() == engine::EventType::KeyPressed) {
			engine::KeyPressedEvent& e = (engine::KeyPressedEvent&)event;
			APP_TRACE("{0}", (char)e.getKeyCode());
		}
	}

	void onAttach() override {
		
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
		
	}				// Commands to be executed on attachment to stack 

	void onDetach() override {}				// Commands to be executed on detachment

private:
	// Rendering API
	engine::u_Ptr<engine::Renderer> s_Renderer;
	engine::ShaderLibrary m_ShaderLibrary;			// For handling of shader program files

	// Camera
	engine::OrthoCameraController m_CameraController;

	// Generation of shaders using vertex array testing
	engine::s_Ptr<engine::VertexArray> m_SquareVertexArray;

	glm::vec4 m_SquareColor1 = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareColor2 = { 0.2f, 0.3f, 0.8f, 1.0f };
};

*/

/*
	An example application class
*/
class Sandbox : public engine::AppFrame {
public:
	Sandbox() : engine::AppFrame() {
		//pushLayer(NEW ExampleLayer());
		pushLayer(NEW SandboxLayer);
	}


	Sandbox(engine::WindowSpecs specs) : engine::AppFrame(specs) {
		//pushLayer(new ExampleLayer());
		pushLayer(NEW SandboxLayer);
	}


	~Sandbox() {

	}
};

/*
	Returns a new application sandbox defined and executed engine side.
*/
engine::u_Ptr<engine::AppFrame> engine::createApp() {
	auto windowSpecs = engine::WindowSpecs("Sandbox", 1600, 900);

	return engine::m_UPtr<Sandbox>(windowSpecs);
}