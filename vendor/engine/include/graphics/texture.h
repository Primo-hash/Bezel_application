/*
	Storage class for handling and storing textures
*/
#pragma once

#include "engine/precompiled.h"
#include "engine/include/core.h"
#include "engine/include/logger.h"

#include <glad/glad.h>
#include <engine/vendor/stb/src/stb_image.h>	// Raw image loading

namespace engine {

	/*
		Texture abstractions
	*/
	class Texture {
	public:
		Texture(uint32_t width, uint32_t height);
		Texture(const std::string& path);
		~Texture();

		void setData(void* data, uint32_t size);

		uint32_t getWidth() const { return m_Width; }
		uint32_t getHeight() const { return m_Height; }

		uint32_t getID() const { return m_RendererID; }

		void bind(uint32_t slot = 0) const;
		// Custom operator allowing the comparison of two textures
		bool operator==(const Texture& other) const { return m_RendererID == ((Texture&)other).m_RendererID; }
	private:
		// Texture's internal RGB8 format and OpenGL's simplified RGB format
		GLenum m_InternalFormat, m_DataFormat;

		std::string m_Path;				// Filepath
		uint32_t m_Width, m_Height;		// Texture specs
		uint32_t m_RendererID;
	};

}