/*
	This class handles OpenGL rendering options and initializes
	the internal renderers for application use
*/
#pragma once

#include "engine/precompiled.h"
#include "engine/include/core.h"
#include "vertex-array.h"	// Utilizes GLAD

#include <glm/glm.hpp>

namespace engine {

	class RenderAPI {
	public:
		RenderAPI();
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		virtual void setClearColor(const glm::vec4& color);
		virtual void clear();
		virtual void drawIndexed(const s_Ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		virtual void drawVAO(GLuint& VAO, unsigned int size);
		virtual void drawVAOInstanced(GLuint& VAO, unsigned int size, unsigned int num_instances);
	};

}
