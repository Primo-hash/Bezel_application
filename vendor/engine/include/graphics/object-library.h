/*
	The class defined in this header manipulate the tinyobjloader library to read and store
	objects for 3D rendering
*/
#pragma once
#include "engine/precompiled.h"
#include "engine/include/logger.h"
#include "3D-processing/mesh-data.h"

#include <glad/glad.h>
#include <tiny_obj_loader.h>

namespace engine {

	/*
		Object library class for better overview and organization of objects on runtime
	*/
	class ObjectLibrary {
	public:
		ObjectLibrary();
		void add(const std::string& name, MeshStore meshObject);
		void add(const std::string& name, RawShape shapeObject);
		void loadObjectFromFile(const std::string& name, const std::string& filepath);

		//MeshStore get(const std::string& name);
		RawShape get(const std::string& name);

		bool exists(const std::string& name) const;
		
	private:
		std::unordered_map<std::string, MeshStore> m_MeshObjects;
		std::unordered_map<std::string, RawShape> m_RawShapeObjects;
	};

}