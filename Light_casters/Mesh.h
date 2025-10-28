#pragma once

#include "Geometry.h"

class Mesh
{
public:
	Mesh(const Geometry& geometry);
	~Mesh();

	void draw() const;
	
private:
	GLuint VAO, VBO;
	GLsizei vertexCount;
};

