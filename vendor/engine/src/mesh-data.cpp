#include "engine/include/graphics/3D-processing/mesh-data.h"

namespace engine {
	
	RawShape::RawShape() {}
	RawShape::RawShape(tinyobj::attrib_t a, std::vector<tinyobj::shape_t> s, std::vector<tinyobj::material_t> m) : attrib(a), shapes(s), materials(m) {}

	ShapeIndices::ShapeIndices(int posIndex, int normIndex, int texCIndex) :
		positionIndex(posIndex),
		normalIndex(normIndex),
		texCoordIndex(texCIndex) {
	}

	ShapeIndices::ShapeIndices() :
		positionIndex(0),
		normalIndex(0),
		texCoordIndex(0) {
	}

	Shape::Shape(int polyCount) : polygonCount(polyCount) {}

	Shape::Shape() : polygonCount(0) {}

	Mesh::Mesh() {
	}

	int Mesh::getVertexCount() const {
		return m_Positions.size();
	}

	MeshStore::MeshStore() : m_Shapes(0) {

	}

	MeshStore::MeshStore(std::string filepath) : m_Shapes(0) {
		// Extracting name from file path
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();	// File name stripped of extension (file type)
	}

	std::vector<s_Ptr<Mesh>>& MeshStore::buildMeshes() {
		m_Meshes.clear();
		m_MeshMaterialIDs.clear();
		if (m_ShapeCount == 0) { return m_Meshes; }	// Error handling empty 3d object

		for (int i = 0; i < m_Shapes.size(); i++) {
			std::unordered_map<int, s_Ptr<Mesh>> parts;

			for (int j = 0; j < m_Shapes[i].polygonCount; j++) {

				s_Ptr<Mesh> mesh;
				int materialID = m_Shapes[i].polygonIndex[j];	// Mapping indices
				if (parts.find(materialID) != parts.end()) {	// Continue from last mesh
					mesh = parts[materialID];
				}
				else {	// Set new mesh
					mesh = m_SPtr<Mesh>();
					parts[materialID] = mesh;
				}

				for (int k = 0; k < 3; k++) {

					int index = m_Shapes[i].shapeIndices[j * 3 + k].positionIndex;
					if (index < m_Positions.size()) {
						mesh->m_Positions.push_back(m_Positions[index]);
					}

					index = m_Shapes[i].shapeIndices[j * 3 + k].normalIndex;
					if (index < m_Normals.size()) {
						mesh->m_Normals.push_back(m_Normals[index]);
					}

					index = m_Shapes[i].shapeIndices[j * 3 + k].texCoordIndex;
					if (index < m_TexCoords.size()) {
						mesh->m_TexCoords.push_back(m_TexCoords[index]);
					}

					mesh->m_RawIndexBuffer.push_back(mesh->m_Positions.size() - 1);
				}

			}

			std::unordered_map<int, s_Ptr<Mesh>>::iterator it = parts.begin();

			// Iterate over the map using iterator
			while (it != parts.end())
			{
				m_Meshes.push_back(it->second);
				m_MeshMaterialIDs.push_back(it->first);
				it++;
			}


		}

		return m_Meshes;
	}
}
