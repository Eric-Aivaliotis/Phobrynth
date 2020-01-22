#pragma once
/*
	Object Loader which will load objects into opengl
	Author: Shawn Matthews
	Date: October, 2019

	Edited by: Christopher Grigor
	Date: December 2019
	Only minor changes made
*/

#include "Mesh.h"
#include <vector>

struct MeshData {
	//Vertex data 
	std::vector<Vertex> vertices;

	//index data 
	std::vector<uint32_t> indices;
};

//Everything here will be public
class Objectloader {
public:
	//Loads a mesh from an obj file at the given file name
	//filename (the path to the file to load), baseColor (the value set for the vertex color attribute)
	//then returns the mesh data loaded from the .obj file
	static MeshData LoadObject(const char* fileName, glm::vec4 baseColor = glm::vec4(1.0f));
	//Loads mesh from obj file, and immediately creates an OpenGl mesh from it
	//filename (the path to the file to load), baseColor (the value set for the vertex color attribute)
	//returns a mesh created from the data loaded from the .obj file
	static Mesh::Sptr LoadObjectToMesh(const char* fileName, glm::vec4 baseColor = glm::vec4(1.0f)) {
		MeshData data = LoadObject(fileName, baseColor);

		return std::make_shared<Mesh>(data.vertices.data(), data.vertices.size(),
			data.indices.data(), data.indices.size());
	}
};