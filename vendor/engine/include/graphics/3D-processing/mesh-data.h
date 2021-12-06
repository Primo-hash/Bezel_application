/*
	The classes defined in this header store and process tinyobjloader processed data
*/
#pragma once
#include "engine/precompiled.h"
#include "engine/include/logger.h"

#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

	struct RawShape {
	public:
		RawShape();
		RawShape(tinyobj::attrib_t a, std::vector<tinyobj::shape_t> s, std::vector<tinyobj::material_t> m);

	public:
		//Some variables that we are going to use to store data from tinyObj
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.
	};

	struct ShapeIndices {
	public:
		ShapeIndices(int posIndex, int normIndex, int texCIndex);
		ShapeIndices();

	public:
		int positionIndex;
		int normalIndex;
		int texCoordIndex;
	};

	struct Shape {
	public:
		Shape(int polyCount);
		Shape();

	public:
		std::vector<ShapeIndices>		shapeIndices;	// Holds index in color, position and normals 
		std::vector<int>                polygonIndex;	// A polygon in this case is a collection of indices
		int polygonCount;
	};

	class Mesh {
	public:
		Mesh();
		int getVertexCount() const;

	public:
		std::vector<glm::vec3>          m_Positions;
		std::vector<glm::vec3>          m_Normals;
		std::vector<glm::vec2>          m_TexCoords;
		std::vector<unsigned int>	    m_RawIndexBuffer;
	};


	class MeshStore {
	public:
		MeshStore();
		MeshStore(std::string filepath);
		std::vector<s_Ptr<Mesh>>& buildMeshes();

	public:
		std::string						m_Name;
		std::vector<glm::vec3>          m_Positions;
		std::vector<glm::vec3>          m_Normals;
		std::vector<glm::vec2>          m_TexCoords;

		std::vector<Shape>				m_Shapes;
		std::vector<s_Ptr<Mesh>>		m_Meshes;
		std::vector<int>                m_MeshMaterialIDs;
		int                             m_ShapeCount;
	};

}