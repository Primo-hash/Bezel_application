#include "engine/include/graphics/shader.h"

namespace engine {

	// HELPER FUNCTIONS

	/*
		Returns GL shader type from string
	*/
	static GLenum shaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		ENGINE_ASSERT(false, "Unknown shader type when extracting from string!");
		return 0;
	}

	// SHADER CLASS

	/*
		Constructor reads shader programs from file
	*/
	Shader::Shader(const std::string& filepath) {
		std::string source = readFile(filepath);
		auto shaderSources = preProcess(source);
		compile(shaderSources);

		// Extracting name from file path
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();	// File name stripped of extension (file type)
	}

	/*
		Constructor reads shader programs from strings
	*/
	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	/*
		Free memory on shader program destruction destruction
	*/
	Shader::~Shader() {
		glDeleteProgram(m_RendererID);
	}

	void Shader::bind() const {
		glUseProgram(m_RendererID);
	}

	void Shader::unbind() const {
		glUseProgram(0);
	}

	/*
		Shader API for assigning datatypes to the shader program.
		For example addUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		To set the viewprojection.
	*/

	void Shader::addUniformInt(const std::string& name, int value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	// Supports adding an array of ints for texture sampling 
	// Required for batch rendering
	void Shader::addUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::addUniformFloat(const std::string& name, float value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::addUniformFloat2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::addUniformFloat3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::addUniformFloat4(const std::string& name, const glm::vec4& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::addUniformVec2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1,  &value[0]);
	}

	void Shader::addUniformVec3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, &value[0]);
	}

	void Shader::addUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::addUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	/*
		Reads shader language files e.g. .glsl
	*/
	std::string Shader::readFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			// ptr to end of file (iterator value now also size of file)
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();	// size of file
			if (size != -1) {
				// resize result string to size of file using ptr iterator
				result.resize(size);
				// ptr back to beginnning
				in.seekg(0, std::ios::beg);
				// read file into string
				in.read(&result[0], size);
				// close file
				in.close();
			}
			else {		// Empty file error handling
				ENGINE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else {	// Invalid/Non-existent file path error handling
			ENGINE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	/*
		Support for '#type' use in glsl, this function parses and returns
		an unordered map containing every #type defined in a glsl file and the
		shader program they define. This allows us for example to define
		fragment shaders and vertex shaders in the same file.
		Function continues until end of file.
		Map key as GLenum for compatibility with GL parsing of shader programs.

	*/
	std::unordered_map<GLenum, std::string> Shader::preProcess(const std::string& source) {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);		// Beginning of shader #type declaration line
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);		// End of shader #type declaration line
			ENGINE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;	// Beginning of shader type name, after #type
			std::string type = source.substr(begin, eol - begin);
			ENGINE_ASSERT(shaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); // Beginning of shader program after shader #type declaration
			ENGINE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); // Beginning of next shader #type declaration

			shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	/*
		Compiles a shader program using preprocessed shader source code.
		Shader compilation copied from GL wiki.
	*/
	void Shader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		GLuint program = glCreateProgram();
		ENGINE_ASSERT(shaderSources.size() <= 2, "Max 2 shaders supported currently (vertex and fragment types)");
		std::array<GLenum, 2> glShaderIDs;	// Changed to array from vector for better performance
		int glShaderIDIndex = 0;

		for (auto& it : shaderSources) {	// Compile every shader source provided in map
			GLenum type = it.first;
			const std::string& source = it.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();

			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {	// Error handling
				GLint maxLength = 0;

				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);
				ENGINE_ERROR("{0}", infoLog.data());	// Error logging
				ENGINE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Set current renderer address to be the compiled shader program
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) { // Linking Error handling
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			ENGINE_ERROR("{0}", infoLog.data());	// Error logging
			ENGINE_ASSERT(false, "Shader link failure!");
			return;
		}

		/*
			Free memory on end
		*/
		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	// SHADER LIBRARY CLASS

	/*
		Constructor
	*/
	ShaderLibrary::ShaderLibrary() {
		
	}

	/*
		Adds shader to library with name
	*/
	void ShaderLibrary::add(const std::string& name, const s_Ptr<Shader>& shader) {
		ENGINE_ASSERT(!exists(name), "Shader already exists in library!");
		m_Shaders[name] = shader;
	}

	/*
		Adds shader to library after extracting file name
	*/
	void ShaderLibrary::add(const s_Ptr<Shader>& shader) {
		auto& name = shader->getName();
		add(name, shader);
	}

	/*
		Loads shader from user's specified filepath
		Inserts shader into shader library
	*/
	s_Ptr<Shader> ShaderLibrary::load(const std::string& filepath) {
		auto shader = m_SPtr<Shader>(filepath);
		add(shader);
		return shader;
	}

	/*
		Loads shader from user's specified filepath and creates custom name
		Inserts shader into shader library
	*/
	s_Ptr<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) {
		auto shader = m_SPtr<Shader>(filepath);
		add(name, shader);
		return shader;
	}

	/*
		Returns shader from shader lib map, if it exists
	*/
	s_Ptr<Shader> ShaderLibrary::get(const std::string& name)
	{
		ENGINE_ASSERT(exists(name), "Shader not found in library!");
		return m_Shaders[name];
	}

	/*
		Returns if a shader already exists in library by name
	*/
	bool ShaderLibrary::exists(const std::string& name) const {
		return m_Shaders.find(name) != m_Shaders.end();
	}

}
