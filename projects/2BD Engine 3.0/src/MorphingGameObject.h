#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <memory> // Needed for smart pointers
#include "Mesh.h"

class MorphingGameObject {
public:
	// Shorthand for shared_ptr
	typedef std::shared_ptr<MorphingGameObject> Sptr;

	// Creates a new mesh from the given vertices and indices
	MorphingGameObject(Mesh &mesh1, Mesh &mesh2);
	~MorphingGameObject();

	// Draws this mesh
	void Draw();

	/*void UpdateData(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);

	GLuint getMyVao();

	GLuint* getMyBuffers();*/


private:
	// Our GL handle for the Vertex Array Object
	GLuint vaoOfMorphing;
	// 0 is vertices, 1 is indices
	GLuint BuffersOfMorphing[2];
	// The number of vertices and indices in this mesh
	size_t myVertexCount, myIndexCount;

	float time=0;
};

