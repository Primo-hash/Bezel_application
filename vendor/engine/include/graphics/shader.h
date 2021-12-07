/*
	The classes and structures defined in this header are part of the vertex shader program/preprocessing component of the
	OpenGL pipeline.
*/
#pragma once

#include "engine/precompiled.h"
#include "engine/include/logger.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

	/*
		Shader class to be implemented for getting and composing shaders from shared sources
	*/
	class Shader {
	public:
		Shader(const std::string& filepath);
		Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		virtual void bind() const;
		virtual void unbind() const;

		virtual const std::string& getName() const { return m_Name; }

		/*
			Data type addition to shader program
		*/
		void addUniformInt(const std::string& name, int value);
		void addUniformIntArray(const std::string& name, int* values, uint32_t count);
		void addUniformFloat(const std::string& name, float value);
		void addUniformFloat2(const std::string& name, const glm::vec2& value);
		void addUniformFloat3(const std::string& name, const glm::vec3& value);
		void addUniformFloat4(const std::string& name, const glm::vec4& value);

		void addUniformVec3(const std::string& name, const glm::vec3& value);
		void addUniformVec2(const std::string& name, const glm::vec2& value);

		void addUniformMat3(const std::string& name, const glm::mat3& matrix);
		void addUniformMat4(const std::string& name, const glm::mat4& matrix);
	
	private:
		uint32_t m_RendererID;
		std::string m_Name;

		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};

	/*
		Shader library class for better overview and organization of shaders on runtime
	*/
	class ShaderLibrary {
	public:
		ShaderLibrary();
		void add(const std::string& name, const s_Ptr<Shader>& shader);
		void add(const s_Ptr<Shader>& shader);
		s_Ptr<Shader> load(const std::string& filepath);
		s_Ptr<Shader> load(const std::string& name, const std::string& filepath);

		s_Ptr<Shader> get(const std::string& name);

		bool exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, s_Ptr<Shader>> m_Shaders;
	};

}