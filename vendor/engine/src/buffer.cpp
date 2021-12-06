#include "engine/include/graphics/buffer.h"

/*
	Vertex and index buffering specific to OpenGL
*/

namespace engine {

	/*
		VERTEX BUFFER DEF
	*/

	/*
		Assign memory and space based on size for constructed vertex buffer at renderer address
		Used for single object draw calls utilizing static buffer
	*/
	VertexBuffer::VertexBuffer(const void* vertices, unsigned int size) {
		glCreateBuffers(1, &m_RendererID);					// OpenGL generation of buffer and assigning it an ID
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	/*
		Assign memory and space based on size for constructed vertex buffer at renderer address
		Used for single object draw calls utilizing static buffer
	*/
	template <typename T>
	VertexBuffer::VertexBuffer(std::vector<T>& vertices, unsigned int size) {
		glCreateBuffers(1, &m_RendererID);					// OpenGL generation of buffer and assigning it an ID
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);
	}

	/*
		Assign memory and space based on size for constructed vertex buffer at renderer address
		Used for single/multiple object draw calls utilizing dynamic buffer
	*/
	VertexBuffer::VertexBuffer(unsigned int size) {
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	/*
		Free vertex memory on destruction
	*/
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	/*
		Engine application of glBindBuffer for binding vertices at current renderer address
	*/
	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	/*
		Engine application of glBindBuffer for unbinding vertices
	*/
	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
		Binds all buffers stored in an array of Vertex objects.
		To be utilized mostly by renderer before issuing a single draw call
		for batch rendering
	*/
	void VertexBuffer::setData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/*
		INDEX BUFFER DEF
	*/

	/*
		Assign memory and space based on size for constructed index buffer at renderer address
	*/
	/*
	IndexBuffer::IndexBuffer(std::vector<engine::PolyVertex>& vertices, unsigned int count) : m_Count(count) {
		glCreateBuffers(1, &m_RendererID);					// OpenGL generation of buffer and assigning it an ID if not assigned
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(PolyVertex), vertices.data(), GL_STATIC_DRAW);
	}
	*/

	/*
		Assign memory and space based on size for constructed index buffer at renderer address
	*/
	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count) {
		glCreateBuffers(1, &m_RendererID);					// OpenGL generation of buffer and assigning it an ID if not assigned
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	/*
		Free index memory on destruction
	*/
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	/*
		Engine application of glBindBuffer for binding indices at current renderer address
	*/
	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	/*
		Engine application of glBindBuffer for unbinding indices
	*/
	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}