#include "engine/include/graphics/renderer.h"
#include "engine/include/graphics/storage.h"

namespace engine {
	/*
		Static members
	*/
	s_Ptr<RenderAPI> Renderer::s_RenderAPI = m_SPtr<RenderAPI>();
	engine::ObjectLibrary* Renderer::s_ObjectLibrary = NEW engine::ObjectLibrary();
	engine::ShaderLibrary* Renderer::s_ShaderLibrary = NEW engine::ShaderLibrary();
	static RendererStorage s_Data;
	static RendererStorage3D s_3DData;
	static DepthMapStorage s_ShadowMap;

	/*
		Sets up storage components with engine specific specs of quads, shader and textured quads
		Along with adapting usage to feeding buffer multiple objects before issuing draw.
	*/
	Renderer::Renderer() {
		// Init 3D shader before begin scene
		s_3DData.lightingShader = s_ShaderLibrary->load("assets/shaders/lighting-shader.glsl");

		// Configurate the shadow map
		configDepthMap();

		/*
			DATA DEFINITION FOR QUAD DRAWING
		*/
		s_Data.quadVertexArray = m_SPtr<VertexArray>();
		//s_Data.quadVertexBufferStore = NEW QuadVertex[s_Data.MAXVERTICES];
		s_Data.quadVertexBufferStore = m_SPtr<std::vector<QuadVertex>>(s_Data.MAXVERTICES);
		// Vertex default positioning
		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Batch rendering
		s_Data.quadVertexBuffer = m_SPtr<VertexBuffer>(s_Data.MAXVERTICES * sizeof(QuadVertex));
		s_Data.quadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4,	 "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float,     "a_TexID" },
			{ ShaderDataType::Float, "a_TileCount" }
			});
		s_Data.quadVertexArray->setVertexBuffer(s_Data.quadVertexBuffer);

		// Index
		uint32_t* quadIndices = NEW uint32_t[s_Data.MAXINDICES];
		// Set indices to ptr
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MAXINDICES; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
		
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Ptr<IndexBuffer> quadIndexBuffer = m_SPtr<IndexBuffer>(quadIndices, s_Data.MAXINDICES);
		s_Data.quadVertexArray->setIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		/*
			DATA DEFINITION FOR TEXTURED DRAWING
			Used for generating colored squares by generating textures instead of importing
		*/
		s_Data.whiteTexture = m_SPtr<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;		// Texture color defaults to white
		s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
		
		int32_t samplers[s_Data.MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < s_Data.MAXTEXTURESLOTS; i++) {	// Set texture sampler ID's for shader program
			samplers[i] = i;
		}

		// Uploading shader program for textures
		s_Data.textureShader = m_SPtr<Shader>("assets/shaders/texture.glsl");
		s_Data.textureShader->bind();
		s_Data.textureShader->addUniformIntArray("u_Textures", samplers, s_Data.MAXTEXTURESLOTS);
	
		s_3DData.lightingShader->bind();
		s_3DData.lightingShader->addUniformIntArray("u_Textures", samplers, s_Data.MAXTEXTURESLOTS);

		// Default texture slot to be used will have id 0
		s_Data.textureSlots[0] = s_Data.whiteTexture;
	}

	Renderer::~Renderer() {
		//delete[] s_Data.quadVertexBufferStore;
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		s_RenderAPI->setViewport(0, 0, width, height);
	}

	/*
		Sets all required values before:
			- Setting view projection of camera
			- issuing a batch rendering request
	*/
	void Renderer::beginScene(OrthographicCamera& camera) {
		s_Data.viewProjectionMatrix = camera.getViewProjectionMatrix();

		s_Data.textureShader->bind();
		s_Data.textureShader->addUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	
		s_Data.quadIndexCount = 0;									 // Index init on scene beginning
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferStore;   // Points to array of quad vertex objects
	
		s_Data.textureSlotIndex = 1; // Index starts at 1 since default texture inserted in constructor
	}

	/*
		Sets all required values before:
			- Setting view projection of camera
			- issuing a batch rendering request
	*/
	void Renderer::beginScene(PerspectiveCamera& camera) {
		s_Data.viewProjectionMatrix = camera.getViewProjectionMatrix();

		// Depth of scene variables to depth shader
		// Orthographic projection to capture the whole scene
		s_ShadowMap.lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, s_ShadowMap.NEAR_PLANE, s_ShadowMap.NEAR_PLANE);
		s_ShadowMap.lightView = glm::lookAt(camera.getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		s_ShadowMap.lightSpaceMatrix = s_ShadowMap.lightProjection * s_ShadowMap.lightView;


		s_3DData.lightingShader->bind();
		// In Vertex shader ViewProjection Matrix
		s_3DData.lightingShader->addUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		s_3DData.lightingShader->addUniformMat4("u_LightSpaceMatrix", s_ShadowMap.lightSpaceMatrix);
		// In Fragment shader lighting
		s_3DData.lightingShader->addUniformVec3("u_LightColor", { 1.0f, 1.0f, 1.0f });
		s_3DData.lightingShader->addUniformVec3("u_LightPosition", camera.getPosition());
		s_3DData.lightingShader->addUniformVec3("u_ViewPosition", camera.getPosition());
		// In Fragment shader shadows
		s_3DData.lightingShader->addUniformInt("u_DiffuseTexture", 0);
		s_3DData.lightingShader->addUniformInt("u_ShadowMap", 1);

		// Bind and add space matrix
		s_ShadowMap.depthShader->bind();
		s_ShadowMap.depthShader->addUniformMat4("u_LightSpaceMatrix", s_ShadowMap.lightSpaceMatrix);

		// 2D shaders
		s_Data.textureShader->bind();
		s_Data.textureShader->addUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		

		s_Data.quadIndexCount = 0;									 // Index init on scene beginning
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferStore;   // Points to array of quad vertex objects

		s_Data.textureSlotIndex = 1; // Index starts at 1 since default texture inserted in constructor
	}

	/*
		When scene ends all stored vertex buffers are issued in one draw call
	*/
	void Renderer::endScene() {
		if (s_Data.quadIndexCount == 0 && s_3DData.vertices.size() == 0) {	// Nothing to draw
			return;
		}

		// SHADOWMAP RENDER
		s_RenderAPI->setViewport(0, 0, s_ShadowMap.WIDTH, s_ShadowMap.HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, s_ShadowMap.depthMapFBO);
		s_RenderAPI->clear();
		// Bind only as many textures as inserted by engine and application
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
			s_Data.textureSlots[i]->bind(i);
		}
		GLuint VAO = compileModel(s_3DData.vertices);
		submit(s_ShadowMap.depthShader, VAO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Reset scene
		AppFrame& appInstance = AppFrame::get();
		s_RenderAPI->setViewport(0, 0, appInstance.getWindow().getWidth(), appInstance.getWindow().getHeight());
		s_RenderAPI->clear();

		// ACTUAL 3D RENDER
		//submit(s_3DData.lightingShader, s_3DData.polyVertexArray);	// executes draw with custom shader
		submit(s_3DData.lightingShader, VAO);	// executes draw with custom shader

		s_3DData.vertices.clear();	// VERY IMPORTANTE
		s_3DData.vertexCount = 0;

		s_3DData.polyVertexBuffer.reset();
		s_3DData.polyIndexBuffer.reset();
		s_3DData.polyVertexArray.reset();

		// clean VAO
		cleanVAO(s_3DData.VAO);

		// RESET quad ptrs
		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferStore;
		s_Data.textureSlotIndex = 1;

		// Clear textures
		glBindTexture(GL_TEXTURE_2D, 0);	// Remove binding
	}

	/*
		For submitting application custom object draw calls to renderer, will override batch rendering calls
	*/
	void Renderer::submit(const s_Ptr<Shader>& shader, const s_Ptr<VertexArray>& vertexArray, const glm::mat4& transform) {
		// How to render
		shader->bind();

		// Following dynamic ptr is ok to use in this case since renderer is dependent on OpenGL.
		shader->addUniformMat4("u_ViewProjection", s_Data.viewProjectionMatrix);

		// What to render
		vertexArray->bind();
		s_RenderAPI->drawIndexed(vertexArray);
	}

	/*
		For submitting application custom object draw calls to renderer, will override batch rendering calls
	*/
	void Renderer::submit(const s_Ptr<Shader>& shader, GLuint &VAO) {
		int32_t samplers[s_Data.MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < s_Data.MAXTEXTURESLOTS; i++) {	// Set texture sampler ID's for shader program
			samplers[i] = i;
		}

		// How to render
		shader->bind();

		// What to render
		s_RenderAPI->drawVAO(VAO, s_3DData.vertexCount);
	}

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

	/*
		Draw quad with a 2D position and color
	*/
	void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	/*
		Draw quad with a 3D position, color and custom texture coords
	*/
	void Renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		const float texID = 0.f;		// Default texture
		const float tileCount = 1.f;    // Default tile count
		
		// Send draw call to engine if index count maxed out before continuing
		// Endscene will reset all ptrs after drawing
		if (s_Data.quadIndexCount >= s_Data.MAXINDICES) {
			endScene();
		}

		// Transform vertices to position then spread vertices to each quad corner
		// using TRS method
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		/*
		// Iterate and set attributes of quad in vertex buffer
		for (uint32_t i = 0; i < s_Data.QUADVERTEXCOUNT; i++)
		{
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = s_Data.textureCoordMapping[i];
			s_Data.quadVertexBufferPtr->texID = texID;
			s_Data.quadVertexBufferPtr->tileCount = tileCount;
			s_Data.quadVertexBufferPtr++;
		}
		*/

		int idx = 0;

		// Iterate and set attributes of quad in vertex buffer
		for (auto it = s_Data.quadVertexBufferPtr->begin(); idx < s_Data.QUADVERTEXCOUNT; it++) {
			it->position = transform * s_Data.quadVertexPositions[idx];
			it->color = color;
			it->texCoord = s_Data.textureCoordMapping[idx];
			it->texID = texID;
			it->tileCount = tileCount;
			idx++;
		}
			  
		s_Data.quadIndexCount += 6;	// Increment index count for potential next buffer
	}

	/*
		Draw quad with a 2D position, tilecount and texture
	*/
	void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const s_Ptr<Texture>& texture, float tileCount, const glm::vec4& tintColor) {
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tileCount, tintColor);
	}

	/*
		Draw textured quad with a 3D position, tilecount
	*/
	void Renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const s_Ptr<Texture>& texture, float tileCount, const glm::vec4& tintColor) {
		float texID = 0.f;		// Default texture

		// Send draw call to engine if index count maxed out before continuing
		// Endscene will reset all ptrs after drawing
		if (s_Data.quadIndexCount >= s_Data.MAXINDICES) {
			endScene();
		}

		// Check if texture application has sent as a param matches an existing ID
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
			// Using custom Texture object operator to compare renderer IDs
			if (*s_Data.textureSlots[i].get() == *texture.get()) {
				texID = (float)i;
				break;
			}
		}

		// If no textures match, texture is added to texture slots array
		if (texID == 0.f) {
			texID = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		// Transform vertices to position then spread vertices to eeach quad corner
		// using TRS method
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		/*
		// Iterate and set attributes of quad in vertex buffer
		for (uint32_t i = 0; i < s_Data.QUADVERTEXCOUNT; i++)
		{
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = s_Data.DEFAULTCOLOR;
			s_Data.quadVertexBufferPtr->texCoord = s_Data.textureCoordMapping[i];		// BL
			s_Data.quadVertexBufferPtr->texID = texID;
			s_Data.quadVertexBufferPtr->tileCount = tileCount;
			s_Data.quadVertexBufferPtr++;
		}
		*/
		
		int idx = 0;

		// Iterate and set attributes of quad in vertex buffer
		for (auto it = s_Data.quadVertexBufferPtr->begin(); idx < s_Data.QUADVERTEXCOUNT; it++) {
			it->position = transform * s_Data.quadVertexPositions[idx];
			it->color = s_Data.DEFAULTCOLOR;
			it->texCoord = s_Data.textureCoordMapping[idx];
			it->texID = texID;
			it->tileCount = tileCount;
			idx++;
		}
		
		s_Data.quadIndexCount += 6;	// Increment index count for potential next buffer
	}

	/*
		Draw quad with a 2D position, rotation and color
	*/
	void Renderer::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	/*
		Draw quad with a 3D position, rotation and color
	*/
	void Renderer::drawRotatedQuad(const glm::vec3 position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		float texID = 0.f;		// Default texture
		float tileCount = 1.f;    // Default tile count

		// Send draw call to engine if index count maxed out before continuing
		// Endscene will reset all ptrs after drawing
		if (s_Data.quadIndexCount >= s_Data.MAXINDICES) {
			endScene();
		}

		// Transform vertices to position then spread vertices to each quad corner
		// using TRS method
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		/*
		// Iterate and set attributes of quad in vertex buffer
		for (uint32_t i = 0; i < s_Data.QUADVERTEXCOUNT; i++)
		{
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = s_Data.textureCoordMapping[i];
			s_Data.quadVertexBufferPtr->texID = texID;
			s_Data.quadVertexBufferPtr->tileCount = tileCount;
			s_Data.quadVertexBufferPtr++;
		}
		*/

		int idx = 0;

		// Iterate and set attributes of quad in vertex buffer
		for (auto it = s_Data.quadVertexBufferPtr->begin(); idx < s_Data.QUADVERTEXCOUNT; it++) {
			it->position = transform * s_Data.quadVertexPositions[idx];
			it->color = color;
			it->texCoord = s_Data.textureCoordMapping[idx];
			it->texID = texID;
			it->tileCount = tileCount;
			idx++;
		}

		s_Data.quadIndexCount += 6;	// Increment index count for potential next buffer
	}


	/*
		Draw textured quad with a 2D position, rotation, tilecount
	*/
	void Renderer::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const s_Ptr<Texture>& texture, float tileCount, const glm::vec4& tintColor) {
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileCount, tintColor);
	}

	/*
		Draw textured quad with a 3D position, rotation, tilecount
	*/
	void Renderer::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const s_Ptr<Texture>& texture, float tileCount, const glm::vec4& tintColor) {
		float texID = 0.f;		// Default texture

		// Send draw call to engine if index count maxed out before continuing
		// Endscene will reset all ptrs after drawing
		if (s_Data.quadIndexCount >= s_Data.MAXINDICES) {
			endScene();
		}

		// Check if texture application has sent as a param matches an existing ID
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
			// Using custom Texture object operator to compare renderer IDs
			if (*s_Data.textureSlots[i].get() == *texture.get()) {
				texID = (float)i;
				break;
			}
		}

		// If no textures match, texture is added to texture slots array
		if (texID == 0.f) {
			texID = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		// Transform vertices to position then spread vertices to each quad corner
		// using TRS method
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		/*
		// Iterate and set attributes of quad in vertex buffer
		for (uint32_t i = 0; i < s_Data.QUADVERTEXCOUNT; i++)
		{
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = s_Data.DEFAULTCOLOR;
			s_Data.quadVertexBufferPtr->texCoord = s_Data.textureCoordMapping[i];
			s_Data.quadVertexBufferPtr->texID = texID;
			s_Data.quadVertexBufferPtr->tileCount = tileCount;
			s_Data.quadVertexBufferPtr++;
		}
		*/

		int idx = 0;

		// Iterate and set attributes of quad in vertex buffer
		for (auto it = s_Data.quadVertexBufferPtr->begin(); idx < s_Data.QUADVERTEXCOUNT; it++) {
			it->position = transform * s_Data.quadVertexPositions[idx];
			it->color = s_Data.DEFAULTCOLOR;
			it->texCoord = s_Data.textureCoordMapping[idx];
			it->texID = texID;
			it->tileCount = tileCount;
			idx++;
		}

		s_Data.quadIndexCount += 6;	// Increment index count for potential next buffer
	}

	/*
		Draw circle with a 2D position, size, rotation and quality of circle
		quality in this case is how many times to repeat quads, at some point 
		the quality does not change and so a default of 100 repeats is used.
	*/
	void Renderer::drawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		// Generation stops when a quad has been generated for every degree in a 90 degree angle
		int angleShift = 3;
		float rotation = 0.f;

		for (int i = 0; i < angleShift; i++) {
			drawRotatedQuad(position, size, rotation + i, color);
		}
	}
	
	/*
		Draw circle with a 3D position, size, rotation and quality of circle
		quality in this case is how many times to repeat quads, at some point
		the quality does not change and so a default of 100 repeats is used.
	*/
	void Renderer::drawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		// Generation stops when a quad has been generated for every degree in a 90 degree angle
		int angleShift = 3;
		float rotation = 0.f;

		for (int i = 0; i < angleShift; i++) {
			drawRotatedQuad(position, size, rotation + i, color);
		}
	}

	/*
		Draw textured circle with a 2D position, size, rotation, tint color and quality of circle
		quality in this case is how many times to repeat quads, at some point
		the quality does not change and so a default of 100 repeats is used.
	*/
	void Renderer::drawCircle(const glm::vec2& position, const glm::vec2& size, const s_Ptr<Texture>& texture) {
		// Generation stops when a quad has been generated for every degree in a 90 degree angle
		int angleShift = 3;
		float rotation = 0.f;

		for (int i = 0; i < angleShift; i++) {
			drawRotatedQuad(position, size, rotation + i, texture, 1.f);
		}
	}

	/*
		Draw textured circle with a 3D position, size, rotation, tint color and quality of circle
		quality in this case is how many times to repeat quads, at some point
		the quality does not change and so a default of 100 repeats is used.
	*/
	void Renderer::drawCircle(const glm::vec3& position, const glm::vec2& size, const s_Ptr<Texture>& texture) {
		// Generation stops when a quad has been generated for every degree in a 90 degree angle
		int angleShift = 3;
		float rotation = 0.f;

		for (int i = 0; i < angleShift; i++) {
			drawRotatedQuad(position, size, rotation + i, texture, 1.f);
		}
	}
	
	/*
		Draw textured circle with a 3D position, size, rotation, tint color and quality of circle
		quality in this case is how many times to repeat quads, at some point
		the quality does not change and so a default of 100 repeats is used.
	*/
	void Renderer::draw3DObject(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const glm::vec4& color, const std::string path, const std::string objectName) {
		const float texID = 0.f;										// Default texture
		
		// Transform vertices to position then spread vertices to each polygon corner
		// using TRS method
		/*
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));	// Rotation x-axis
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));	// Rotation y-axis
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));	// Rotation z-axis
		transform = glm::scale(transform, size);												// Scaling

		glm::vec4 posTRS = glm::vec4(position, 1.f) * transform;
		*/
		//loadModel(path, objectName, s_3DData.vertices, {posTRS.x, posTRS.y, posTRS.z}, color, texID);
		loadModel(path, objectName, s_3DData.vertices, position, size, rotation, color, texID);

		/*
		MeshStore meshStore = s_ObjectLibrary->get(objectName);						// get base object mesh for transformation and queue
		std::vector<s_Ptr<Mesh>> meshes = meshStore.buildMeshes();
		std::vector<PolyVertex> vertices;
		std::vector<unsigned int> indices;

		// Transform vertices to position then spread vertices to each polygon corner
		// using TRS method
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *											// Translation
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)) *	// Rotation x-axis
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)) *	// Rotation y-axis
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)) *	// Rotation z-axis
			glm::scale(glm::mat4(1.0f), size);													// Scaling

		if (s_3DData.verticesMap.find(objectName) == s_3DData.verticesMap.end()) {	// New object type gets own vertices
			s_3DData.verticesMap.insert({ objectName, vertices });
			s_3DData.indicesMap.insert({ objectName, indices });
		}

		for (int mesh = 0; mesh < meshes.size(); mesh++) {
			//if (!meshes[mesh]) continue;	// skip to next if empty mesh

			int vertexSize = 0;		// Determine each vertex size based on attributes
			vertexSize += 3;		// Color applied by default
			if (meshes[mesh]->m_Positions.size() != 0)  vertexSize += 3;
			if (meshes[mesh]->m_Normals.size() != 0) vertexSize += 3;
			if (meshes[mesh]->m_TexCoords.size() != 0) vertexSize += 2;

			for (int i = 0; i < meshes[mesh]->getVertexCount(); i++) {
				PolyVertex vertex = PolyVertex();
				glm::vec3 pos = meshes[mesh]->m_Positions[i];	// for shorter alias
				//vertex.position = glm::vec4(pos.x, pos.y, pos.z, 1.0f) * transform;
				vertex.position = pos + position;
				vertex.normal = meshes[mesh]->m_Normals[i];
				vertex.texCoord = meshes[mesh]->m_TexCoords[i];
				vertex.color = color;
				vertex.texID = texID;
				s_3DData.verticesMap[objectName].push_back(vertex);
				//s_3DData.vertices.push_back(vertex);
			}

			// Append each index stored in each mesh to global indices count for batch rendering
			s_3DData.indicesMap[objectName].insert(s_3DData.indicesMap[objectName].end(), meshes[mesh]->m_RawIndexBuffer.begin(), meshes[mesh]->m_RawIndexBuffer.end());
			//s_3DData.indices.insert(s_3DData.indices.end(), meshes[mesh]->m_RawIndexBuffer.begin(), meshes[mesh]->m_RawIndexBuffer.end());
		
		}
		*/
	}

	void Renderer::loadShape(const std::string path, const std::string name) {
		//Some variables that we are going to use to store data from tinyObj
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.

		//Some variables incase there is something wrong with our obj file
		std::string warn;
		std::string err;

		//We use tinobj to load our models. Feel free to find other .obj files and see if you can load them.
		tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + "/" + name + ".obj").c_str(), path.c_str());

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		RawShape rShape = RawShape(attrib, shapes, materials);

		s_ObjectLibrary->add(name, rShape);
	}

	void Renderer::loadModel(std::string path,
		std::string name,
		std::vector<engine::PolyVertex>& vertices,
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec3 rotation,
		glm::vec4 color,
		int texID) {

		RawShape s (s_ObjectLibrary->get(name));	// Copy loaded object from library

		//For each shape defined in the obj file
		/*
		for (auto shape : s.shapes) {
			//We find each mesh
			for (auto meshIndex : shape.mesh.indices) {
				//And store the data for each vertice, including normals
				glm::vec3 vertice = {
					s.attrib.vertices[(double)meshIndex.vertex_index * 3] + position.x,
					s.attrib.vertices[((double)meshIndex.vertex_index * 3) + 1] + position.y,
					s.attrib.vertices[((double)meshIndex.vertex_index * 3) + 2] + position.z
				};
				glm::vec3 normal = {
					s.attrib.normals[(double)meshIndex.normal_index * 3],
					s.attrib.normals[((double)meshIndex.normal_index * 3) + 1],
					s.attrib.normals[((double)meshIndex.normal_index * 3) + 2]
				};
				glm::vec2 textureCoordinate = {                         //These go unnused, but if you want textures, you will need them.
					s.attrib.texcoords[(double)meshIndex.texcoord_index * 2],
					s.attrib.texcoords[((double)meshIndex.texcoord_index * 2) + 1]
				};

				PolyVertex vertex = PolyVertex();
				vertex.position = vertice;
				vertex.normal = normal;
				vertex.texCoord = textureCoordinate;
				vertex.color = color;
				vertex.texID = texID;

				vertices.push_back(vertex); //We add our new vertice struct to our vector
			}
		}
		*/

		//For each shape defined in the obj file
		for (auto shape : s.shapes) {
			//We find each mesh
			for (auto meshIndex : shape.mesh.indices) {
				//And store the data for each vertice, including normals
				glm::vec3 vertice = {
					s.attrib.vertices[(double)meshIndex.vertex_index * 3],
					s.attrib.vertices[((double)meshIndex.vertex_index * 3) + 1],
					s.attrib.vertices[((double)meshIndex.vertex_index * 3) + 2]
				};

				vertice = glm::rotateX(vertice, glm::radians(rotation.x));
				vertice = glm::rotateY(vertice, glm::radians(rotation.y));
				vertice = glm::rotateZ(vertice, glm::radians(rotation.z));

				vertice *= scale;

				glm::vec3 normal = {
					s.attrib.normals[(double)meshIndex.normal_index * 3],
					s.attrib.normals[((double)meshIndex.normal_index * 3) + 1],
					s.attrib.normals[((double)meshIndex.normal_index * 3) + 2]
				};
				glm::vec2 textureCoordinate = {                         //These go unnused, but if you want textures, you will need them.
					s.attrib.texcoords[(double)meshIndex.texcoord_index * 2],
					s.attrib.texcoords[((double)meshIndex.texcoord_index * 2) + 1]
				};

				PolyVertex vertex = PolyVertex();
				vertex.position = vertice + position;
				vertex.normal = normal;
				vertex.texCoord = textureCoordinate;
				vertex.color = color;
				vertex.texID = texID;

				vertices.push_back(vertex); //We add our new vertice struct to our vector
			}
		}
	}

	GLuint Renderer::compileModel(std::vector<PolyVertex>& vertices) {
		//GLuint VAO;
		glGenVertexArrays(1, &s_3DData.VAO);
		glBindVertexArray(s_3DData.VAO);

		//GLuint VBO;
		glGenBuffers(1, &s_3DData.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, s_3DData.VBO);
		
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(PolyVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 13, nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 13, (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 13, (void*)(sizeof(float) * 6));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 13, (void*)(sizeof(float) * 10));
		
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 13, (void*)(sizeof(float) * 12));

		//This will be needed later to specify how much we need to draw. Look at the main loop to find this variable again.
		s_3DData.vertexCount = vertices.size();

		return s_3DData.VAO;
	}

	void Renderer::cleanVAO(GLuint& vao) {
		GLint nAttr = 0;
		std::set<GLuint> vbos;

		GLint eboId;
		glGetVertexArrayiv(vao, GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboId);
		glDeleteBuffers(1, (GLuint*)&eboId);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
		glBindVertexArray(vao);

		for (int iAttr = 0; iAttr < nAttr; ++iAttr)
		{
			GLint vboId = 0;
			glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
			if (vboId > 0)
			{
				vbos.insert(vboId);
			}

			glDisableVertexAttribArray(iAttr);
		}

		for (auto vbo : vbos)
		{
			glDeleteBuffers(1, &vbo);
		}

		glDeleteVertexArrays(1, &vao);
	}

	/*
		Configures the renderer depth map (S_ShadowMap)
	*/
	void Renderer::configDepthMap() {
		// Depth Shader setup
		s_ShadowMap.depthShader = s_ShaderLibrary->load("assets/shaders/depth-shader.glsl");

		// Setup Frame buffer object
		glGenFramebuffers(1, &s_ShadowMap.depthMapFBO);
		s_ShadowMap.depthMap = m_SPtr<Texture>(s_ShadowMap.WIDTH, s_ShadowMap.HEIGHT);
		
		// Attach texture as depth buffer for the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, s_ShadowMap.depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s_ShadowMap.depthMap->getID(), 0);
		glDrawBuffer(GL_NONE);		// Turn off read/write
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	// Free framebuffer
	}
}
