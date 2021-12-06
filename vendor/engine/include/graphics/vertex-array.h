/*
	The classes and structures defined in this header are part of the vertex array definition process
	to render objects using the renderer. These classes need a defined vertex shader to act upon.
*/
#pragma once

#include "buffer.h"		// Utilizes GLAD, precompiled headers and logger from here

namespace engine {

	class VertexArray {
	public:
		VertexArray();
		virtual ~VertexArray();

		virtual void bind() const;
		virtual void unbind() const;

		// Buffer creation
		virtual void setVertexBuffer(const s_Ptr<VertexBuffer>& vertexBuffer);
		virtual void setIndexBuffer(const s_Ptr<IndexBuffer>& indexBuffer);

		// Buffer reference
		virtual const std::vector<s_Ptr<VertexBuffer>>& getVertexBuffers() const { return m_VertexBuffers; };
		virtual const s_Ptr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<s_Ptr<VertexBuffer>> m_VertexBuffers;
		s_Ptr<IndexBuffer> m_IndexBuffer;
	};

}