#include "engine/include/graphics/vertex-array.h"

namespace engine {

	static GLenum shaderDataTypeToOpenGLDataType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		default: ENGINE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
		}
	}

	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::setVertexBuffer(const s_Ptr<VertexBuffer>& vertexBuffer) {
		// Checks that addVertexBuffer is not being called before setLayout
		ENGINE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->getLayout();			// Get premade layout
		for (const auto& element : layout) {					// Send layout specs for every ShaderDataType to GL 
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.getComponentCount(),
				shaderDataTypeToOpenGLDataType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);					// casting to uintptr_t first avoids int to void* size discrepancy
			m_VertexBufferIndex++;								// Increase index created to avoid buffers stacking.
		}

		m_VertexBuffers.push_back(vertexBuffer);				// Add to current collection of Buffers
	}

	void VertexArray::setIndexBuffer(const s_Ptr<IndexBuffer>& indexBuffer) {
		// Bind Indexes for current Array
		glBindVertexArray(m_RendererID);
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;	// Store bound index buffer
	}
}
