/*
	This renderer is built around the concept that it handles different game scenes
	using a camera device, handling of the view on window resize events and handling of
	shaders.
*/
#pragma once
#include "engine/precompiled.h"
#include "engine/include/logger.h"
#include "renderAPI.h"
#include "camera/orthographic-camera.h"
#include "camera/perspective-camera.h"
#include "object-library.h"
#include "shader.h"
#include "texture.h"
#include <tiny_obj_loader.h>


namespace engine {

	/*
	Medium for storing vertex data before processing
*/
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texID;
		float tileCount;
	};

	/*
		Medium for storing vertex data before processing
	*/
	struct PolyVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texID;
	};

	class Renderer {
	public:
		Renderer();
		~Renderer();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(OrthographicCamera& camera);
		static void beginScene(PerspectiveCamera& camera);
		static void endScene();

		static void submit(const s_Ptr<Shader>& shader, const s_Ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void submit(const s_Ptr<Shader>& shader, GLuint& VAO);

		// Returns pointer to renderer instance
		inline static RenderAPI& get() { return *s_RenderAPI; }
		// Returns pointer to object library instance
		static engine::ObjectLibrary* getObjectLibrary() { return s_ObjectLibrary; }
		static engine::ShaderLibrary* getShaderLibrary() { return s_ShaderLibrary; }


		static void loadShape(const std::string path, std::string name);
		static void loadModel(std::string path,
			std::string name,
			std::vector<PolyVertex>& vertices,
			glm::vec3 position,
			glm::vec4 color,
			int texID);

		static GLuint compileModel(std::vector<PolyVertex>& vertices);

		/*
			Following param descriptions:
			position - vec2 x, y || vec3 x, y, z (z used for depth in 2D rendering)
			size - vec2 width, height
			rotation - radians
			color - vec4 RGBA
			texture - Texture object of quad
			tilecount - number of times texture is repeated and fitted to size, defaults to 1
			tintcolor - tinted glass effect for windows or mirrors, defaults to 1 (cool to have, but not needed for project)
		*/

		// Primitives
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		// Primitives w/Texture
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const s_Ptr <Texture>& texture, float tileCount = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const s_Ptr <Texture>& texture, float tileCount = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));

		// Primitives w/Rotation
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3 position, const glm::vec2& size, float rotation, const glm::vec4& color);
		// Primitives w/Texture & Rotation
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const s_Ptr<Texture>& texture, float tileCount = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const s_Ptr<Texture>& texture, float tileCount = 1.f, const glm::vec4& tintColor = glm::vec4(1.0f));
	
		// Circle drawing
		static void drawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		// Textured circle
		static void drawCircle(const glm::vec2& position, const glm::vec2& size, const s_Ptr<Texture>& texture);
		static void drawCircle(const glm::vec3& position, const glm::vec2& size, const s_Ptr<Texture>& texture);

		static void draw3DObject(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const glm::vec4& color, const std::string path, const std::string objectName);
	private:
		static s_Ptr<RenderAPI> s_RenderAPI;
		static engine::ObjectLibrary* s_ObjectLibrary;
		static engine::ShaderLibrary* s_ShaderLibrary;
	};

}