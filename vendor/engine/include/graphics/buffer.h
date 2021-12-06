/*
	The classes and structures defined in this header are part of the vertex specification component of the
	OpenGL pipeline.
*/
#pragma once

#include "engine/precompiled.h"
#include "engine/include/logger.h"
#include <glad/glad.h>

namespace engine {

	/*
		In engine specification of datatypes for use by the buffer layout design
		defined as an enum class to allow similar enum classes called through their scope.
	*/
	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	/*
		Helper function for determining custom datatype sizes (they're the default size)
	*/
	static uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:    return 4;				// Bytes of space this datatype occupies 
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		default: ENGINE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
		}
	}

	/*
		Buffer elements are the components describing the attributes of a shader program
		An example would be position, color or texture coordinates.
		This class transforms the specification of these attributes from hardcoded strings to softcoded
		API calls.
	*/
	struct BufferElement
	{
		std::string name;		// Attribute name
		ShaderDataType type;	// Datatype this attribute represents in the shaderprogram
		uint32_t size;			// Size of this attribute's datatype (selfcalculated)
		size_t offset;			// The attributes assigned offset in the shaderprogram (e.g. 0, 1, 2 ...)
		bool normalized;		// glVertexAttribPointer option using GL_TRUE || GL_FALSE

		BufferElement(ShaderDataType typ, const std::string& nvn, bool norm = false) :
			name(nvn), type(typ),
			size(shaderDataTypeSize(typ)),
			offset(0),							// Offset to be set when all buffer elements in place in layout
			normalized(norm) {}

		uint32_t getComponentCount() const
		{
			switch (type) {
			case ShaderDataType::Float:   return 1;			// Number of components (datatypes)
			case ShaderDataType::Float2:  return 2;         // this datatype contains
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			default: ENGINE_ASSERT(false, "Unknown ShaderDataType!"); return 0;
			}
		}
	};

	/*
		The BufferLayout class handles all Buffer elements for a shader program.
		Having this class allows for calculating the offset and stride between each element
		after they're all defined.
	*/
	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {
			calculateOffsetsAndStride();
		}

		inline uint32_t getStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& getElements() const { return m_Elements; }

		// Set of local or shared iterators for accessing the Buffer elements defined
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

		void calculateOffsetsAndStride() {
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.offset = offset;
				offset += element.size;
				m_Stride += element.size;
			}
		}
	};

	/*
		An interface for buffering vertices
		needs a set buffer layout
		- Utilizes batch rendering
	*/
	class VertexBuffer {
	public:
		VertexBuffer(const void* vertices, unsigned int size);
		template <typename T>
		VertexBuffer(std::vector<T>& vertices, unsigned int size);
		VertexBuffer(unsigned int size);
		virtual ~VertexBuffer();

		virtual void bind() const;
		virtual void unbind() const;


		virtual const BufferLayout& getLayout() const { return m_Layout; }
		virtual void setLayout(const BufferLayout& layout) { m_Layout = layout; }
		virtual void setData(const void* data, uint32_t size);

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	/*
		An interface for buffering indices
		Abstraction defined in graphics lib specific children
	*/
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t * indices, uint32_t count);
		virtual ~IndexBuffer();

		virtual void bind() const;
		virtual void unbind() const;

		virtual uint32_t getCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}