/*
	Header for storage structs used in renderer class
*/
#pragma once
#include "renderer.h"

namespace engine {

	struct DepthMapStorage {
		const unsigned int WIDTH = 1024, HEIGHT = 1024;	 // Depth map resolution
		const float NEAR_PLANE = 1.0f, FAR_PLANE = 7.5f; // Depth map light projection plane spec
		unsigned int depthMapFBO;						 // Depth map frame buffer object
		s_Ptr<Texture> depthMap;						 // Depth map texture
		s_Ptr<Shader> depthShader;						 // Depth map shader

		// Depth map variables to be set from perspective of light
		glm::mat4 lightProjection;
		glm::mat4 lightView;
		glm::mat4 lightSpaceMatrix;
	};

	struct RendererStorage3D {
		const uint32_t MAXPOLYGONS = 1000000;				// Maximum count of polygons to be drawn on single draw call
		const uint32_t MAXPOLYVERTICES = MAXPOLYGONS * 3;	// Maximum polygon count vertices
		const uint32_t MAXPOLYINDICES = MAXPOLYGONS * 3;	// Maximum polygon count indices

		GLuint VAO;
		GLuint VBO;

		std::map<std::string, std::vector<PolyVertex>> verticesMap;
		std::map<std::string, std::vector<unsigned int>> indicesMap;
		std::vector<PolyVertex> vertices;			// Stored vertices for batch rendering	
		unsigned int vertexCount;
		//std::vector<unsigned int> indices;			// Current indices for vertices

		// VERTEX DATA
		s_Ptr<VertexArray> polyVertexArray;			 // Polygon data
		s_Ptr<VertexBuffer> polyVertexBuffer;		 // Custom buffer for single/multiple vertices w/layout handling
		s_Ptr<IndexBuffer> polyIndexBuffer;			 // Custom index buffer
		s_Ptr<Shader> lightingShader;				 // Uploading shaders
	};

	struct RendererStorage {
		static const uint32_t QUADVERTEXCOUNT = 4;			// No. of vertices per quad
		static const uint32_t MAXTEXTURESLOTS = 32;	// Depends on hardware, but pc's should be ok with this maximum
		const uint32_t MAXQUADS = 1000000;			// Maximum count of squares to be drawn on single draw call
		const uint32_t MAXVERTICES = MAXQUADS * 4;	// Maximum square count vertices
		const uint32_t MAXINDICES = MAXQUADS * 6;	// Maximum square count indices
		const glm::vec2 textureCoordMapping[QUADVERTEXCOUNT] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const glm::vec4 DEFAULTCOLOR = { 1.0f, 1.0f, 1.0f, 1.0f };


		// VERTEX DATA
		s_Ptr<VertexArray> quadVertexArray;			 // Quad data
		s_Ptr<VertexBuffer> quadVertexBuffer;		 // Custom buffer for single/multiple vertices w/layout handling
		s_Ptr<Shader> textureShader;				 // Uploading textures
		s_Ptr<Texture> whiteTexture;				 // Generating textures/ flat colors

		uint32_t quadIndexCount = 0;				 // Current index count
		s_Ptr<std::vector<QuadVertex>> quadVertexBufferStore;
		s_Ptr<std::vector<QuadVertex>> quadVertexBufferPtr;
		glm::vec4 quadVertexPositions[4];			 // For applying vertex positions on a loop

		// TEXTURE SLOTS
		// Utilizing std::array since Texture has no default constructor to setup w/partial specialization
		std::array<s_Ptr<Texture>, MAXTEXTURESLOTS> textureSlots;
		uint32_t textureSlotIndex = 1;				 // Index 0 reserved for white texture

		// SCENE DATA
		glm::mat4 viewProjectionMatrix;
	};
}