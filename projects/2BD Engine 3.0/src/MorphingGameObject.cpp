#include "MorphingGameObject.h"

MorphingGameObject::MorphingGameObject(Mesh &mesh1, Mesh &mesh2)
{
	//should be uncommented but currently breaks
	/////myIndexCount = mesh1.getMyIndexCount();
	/////myVertexCount = mesh1.getMyVertexCount();

	// Create and bind our vertex array
	glCreateVertexArrays(1, &vaoOfMorphing);
	glBindVertexArray(vaoOfMorphing);

	// Create 2 buffers, 1 for vertices and the other for indices
	//glCreateBuffers(2, BuffersOfMorphing);
	//Should be uncommented but currently breaks
	/////BuffersOfMorphing[0] = mesh1.getMyBuffers()[0];
	/////BuffersOfMorphing[1] = mesh2.getMyBuffers()[1];

	// Bind and buffer our vertex data
	glBindBuffer(GL_ARRAY_BUFFER, BuffersOfMorphing[0]);
	//glBufferData(GL_ARRAY_BUFFER, mesh1.getMyVertexCount * sizeof(Vertex), , GL_STATIC_DRAW);

	// Bind and buffer our index data
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);


	// Get a null vertex to get member offsets from
	Vertex* vert = nullptr;

	// Enable vertex attribute 0
	glEnableVertexAttribArray(0);
	// Our first attribute is 3 floats, the distance between 
	// them is the size of our vertex, and they will map to the position in our vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Position));

	// Enable vertex attribute 1
	glEnableVertexAttribArray(1);
	// Our second attribute is 4 floats, the distance between 
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &(vert->Color));


	// Enable vertex attribute 1
	glEnableVertexAttribArray(2);
	// Our second attribute is 4 floats, the distance between 
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Normal));

	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), &(vert->UV));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BuffersOfMorphing[1]);

	// Enable vertex attribute 0
	glEnableVertexAttribArray(3);
	// Our first attribute is 3 floats, the distance between 
	// them is the size of our vertex, and they will map to the position in our vertices
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Position));

	// Enable vertex attribute 1
	glEnableVertexAttribArray(4);
	// Our second attribute is 4 floats, the distance between 
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(Vertex), &(vert->Color));


	// Enable vertex attribute 1
	glEnableVertexAttribArray(5);
	// Our second attribute is 4 floats, the distance between 
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(5, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Normal));

	// Unbind our VAO
	glBindVertexArray(0);

}

MorphingGameObject::~MorphingGameObject()
{
}

void MorphingGameObject::Draw()
{
	// Bind the mesh
	glBindVertexArray(vaoOfMorphing);
	if (myIndexCount > 0) {
		// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
		glDrawElements(GL_TRIANGLES, myIndexCount, GL_UNSIGNED_INT, nullptr);
	}
	else {
		// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
		glDrawArrays(GL_TRIANGLES, 0, myVertexCount);
	}
}
