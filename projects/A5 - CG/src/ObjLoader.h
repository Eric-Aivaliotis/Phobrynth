/*
	Handles loading an OBJ file into something readable by OpenGL
	@author Shawn Matthews
	@date   October, 2019
*/
#pragma once

#include "Mesh.h"
#include <vector>

/*
 * Helper structure to store the data required to create a mesh
 */
struct MeshData {
	/*
	 * The vertex data for the mesh
	 */
	std::vector<Vertex>   Vertices;
	/*
	 * The index data for the mesh
	 */
	std::vector<uint32_t> Indices;
};

class ObjLoader {
public:
	/*
	 * Loads a mesh from an obj file at the given file name
	 * @param filename  The path to the file to load
	 * @param baseColor The value to set for the vertex color attribute (default white)
	 * @returns The mesh data loaded from the OBJ file
	 */
	static MeshData LoadObj(const char* filename, glm::vec4 baseColor = glm::vec4(1.0f));
	/*
	 * Loads a mesh from an obj file, and immediately creates an OpenGL mesh from it
	 * @param filename  The path to the file to load
	 * @param baseColor The value to set for the vertex color attribute (default white)
	 * @returns A mesh that has been created from the data loaded from the OBJ file
	 */
	static Mesh::Sptr LoadObjToMesh(const char* filename, glm::vec4 baseColor = glm::vec4(1.0f)) {
		MeshData data = LoadObj(filename, baseColor);
		return std::make_shared<Mesh>(data.Vertices.data(), data.Vertices.size(), data.Indices.data(), data.Indices.size());
	}
};
