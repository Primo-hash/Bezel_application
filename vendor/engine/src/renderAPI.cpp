#include "engine/include/graphics/renderAPI.h"

namespace engine {

	// Static instance of this class

	RenderAPI::RenderAPI() {
		// Causes an access violation error, blend options to be defined in client for now
		// Turn on blending for OpenGL
		//glEnable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
		// When blending use the inverse of src texture to determine drawing priority
		// In this case the destination texture is only drawn if RGBA values of src are 0
		// Allowing "transparency" in textures
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/*
		This function determines object scaling while rendering based on window size
	*/
	void RenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		// Width resizing should reveal more for rendering.
		// Height resizing should scale down the rendered objects.
		glViewport(x, y, width, height);
	}

	/*
		Background color of window
	*/
	void RenderAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	/*
		Clear current draw
	*/
	void RenderAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/*
		Draw vertex array in parameter on screen
		indexCount - Number of elements
	*/
	void RenderAPI::drawIndexed(const s_Ptr<VertexArray>& vertexArray, uint32_t indexCount) {
		// If index count is set, get it from vertex array just to be sure
		uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);	// Remove binding
	}

	/*
		Draw raw vertex array in parameter on screen
	*/
	void RenderAPI::drawVAO(GLuint& VAO, unsigned int size) {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, size);
	}

	/*
		Draw raw vertex array in parameter on screen
	*/
	void RenderAPI::drawVAOInstanced(GLuint& VAO, unsigned int size, unsigned int num_instances) {
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, size, num_instances);
	}
}