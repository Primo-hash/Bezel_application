#include "engine/include/graphics/object-library.h"

namespace engine {

	ObjectLibrary::ObjectLibrary() {

	}

	void ObjectLibrary::add(const std::string& name, MeshStore meshObject) {
		ENGINE_ASSERT(!exists(name), "Object already exists in library!");
		m_MeshObjects[name] = meshObject;
	}

	void ObjectLibrary::add(const std::string& name, RawShape shapeObject) {
		m_RawShapeObjects[name] = shapeObject;
	}

	void ObjectLibrary::loadObjectFromFile(const std::string& name, const std::string& filepath) {
		// Options
		bool triangulate = true;
		int basePath = NULL;		// Will make loader look for .mtl from workinf directory
		
		// INIT model data for referencing
		MeshStore meshObj = MeshStore(filepath);		// Init mesh object
		tinyobj::attrib_t                attributes;			// Init object data for referencing from file
		std::vector<tinyobj::shape_t>    shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;				// Warning handling
		std::string err;				// Error handling

		// LOAD object data from file using tinyobjloader
		tinyobj::LoadObj(&attributes,
						 &shapes,
						 &materials,
						 &warn, 
						 &err, 
						 filepath.c_str(), 
						 filepath.c_str(),
						 triangulate);

		if (!warn.empty()) { ENGINE_WARN("WARNING in Object library loading of {0}: {1}", filepath, warn); }
		if (!err.empty()) { ENGINE_ERROR("ERROR in Object library loading of {0}: {1}", filepath, err); }
		
		// DEFINE the object data through references
		meshObj.m_ShapeCount = (int)shapes.size();

		//For each shape defined in the obj file
		for (int s = 0; s < shapes.size(); s++) {
			meshObj.m_Shapes.push_back(Shape((int)shapes[s].mesh.num_face_vertices.size()));

			// Iterate through polygons
			int offset = 0;		// Index offset
			for (int m = 0; m < shapes[s].mesh.num_face_vertices.size(); m++) {
				// Set the index as a class type
				int sh = shapes[s].mesh.num_face_vertices[m];
				
				meshObj.m_Shapes[s].polygonIndex.push_back(shapes[s].mesh.material_ids[m]);

				// Iterate through vertices in polygon and store in mesh object
				for (int v = 0; v < sh; v++) {
					tinyobj::index_t index = shapes[s].mesh.indices[offset + v];

					meshObj.m_Shapes[s].shapeIndices.push_back(ShapeIndices(
						(int)meshObj.m_Positions.size(),			// Shapeindices are set to each count of current attributes in meshObj
						(int)meshObj.m_Normals.size(),
						(int)meshObj.m_TexCoords.size()));

					if (index.vertex_index * 3 < attributes.vertices.size()) {
						glm::vec3 position(
							attributes.vertices[index.vertex_index * 3],			// Pos X
							attributes.vertices[index.vertex_index * 3 + 1],		// Pos Y
							attributes.vertices[index.vertex_index * 3 + 2]);		// Pos Z
						meshObj.m_Positions.push_back(position);
					}

					if (index.normal_index * 3 < attributes.normals.size()) {
						glm::vec3 normal(
							attributes.normals[index.normal_index * 3],			// Normal X
							attributes.normals[index.normal_index * 3 + 1],		// Normal Y
							attributes.normals[index.normal_index * 3 + 2]);		// Normal Z
						meshObj.m_Normals.push_back(normal);
					}

					if (index.texcoord_index * 2 < attributes.texcoords.size()) {
						glm::vec2 texCoord(
							attributes.texcoords[index.texcoord_index * 2],			// TexCoord X
							attributes.texcoords[index.texcoord_index * 2 + 1]);	// TexCoord Y
						meshObj.m_TexCoords.push_back(texCoord);
					}
				}
				offset += sh;
				//vertices->push_back({ vertice, normal, textureCoordinate }); //We add our new vertice struct to our vector
			}
		}
		
		add(meshObj.m_Name, meshObj);	// Add object to library
	}

	/*
	MeshStore ObjectLibrary::get(const std::string& name) {
		ENGINE_ASSERT(exists(name), "Object not found in library!");
		return m_MeshObjects[name];
	}
	*/

	RawShape ObjectLibrary::get(const std::string& name) {
		return m_RawShapeObjects[name];
	}

	bool ObjectLibrary::exists(const std::string& name) const {
		return m_MeshObjects.find(name) != m_MeshObjects.end();
	}

}
