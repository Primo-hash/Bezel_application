#include "engine/include/graphics/texture.h"


namespace engine {
	
	/*
		Create an empty texture for assigning data at a later stage
	*/
	Texture::Texture(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		// More details in second constructor
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	/*
		Create a texture from an image using stb library
	*/
	Texture::Texture(const std::string& path) : m_Path(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ENGINE_ASSERT(data, "Failed to load image!");	// Error handling no data
		m_Width = width;
		m_Height = height;

		// OpenGL's internal interpretation of formats
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {		// Image uses RGBA
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {	// Image uses RGB
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		ENGINE_ASSERT(internalFormat && dataFormat, "Format unsupported!");		// Error handling formats set

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);						// OpenGL texture init


		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);	// Image internal format and specs bound

		// More filtering details from this source
		// https://gdbooks.gitbooks.io/legacyopengl/content/Chapter7/MinMag.html

		// Nearest neighbor minification filter 
		// Texture looks softer/blurred because filter attempts to scale when zooming camera in or out
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Nearest neighbor magnification filter 
		// keeps image sharp and pixlated, does not attempt to scale when zooming camera in or out
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// More wrapping details from this source
		// https://gdbooks.gitbooks.io/legacyopengl/content/Chapter7/TexParams.html
		// and
		// https://learnopengl.com/Getting-started/Textures

		// Set textures to be tiled per texture coordinate (S, T)
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);	// S equivalent to X coordinate
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);	// T equivalent to Y coordinate

		// For specifying a 2D texture image with its data to OpenGL
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		// Free memory from raw image pixels
		stbi_image_free(data);
	}

	/*
		Free memory
	*/
	Texture::~Texture() {
		glDeleteTextures(1, &m_RendererID);
	}

	/*
		Specify preintialized texture with its data to OpenGL
	*/
	void Texture::setData(void* data, uint32_t size) {
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ENGINE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	/*
		Bind an OpenGL texture to the renderers address
	*/
	void Texture::bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}
}