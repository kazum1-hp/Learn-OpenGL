#include "Mesh.h"



Mesh::Mesh(const Geometry& geometry)
{
	const auto& vertices = geometry.vertices;
	const auto& attributes = geometry.attributes;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	int strideInFloats = 0;
	size_t strideInBytes = 0;
	for (const auto& attr : attributes)
	{
		strideInFloats += attr.size;
		strideInBytes += attr.size * ((attr.type == GL_INT) ? sizeof(int) : sizeof(float));
	}
	vertexCount = static_cast<GLsizei>(vertices.size() / strideInFloats);

	size_t offset = 0;
	for (const auto& attr : attributes)
	{
		glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, (GLsizei)strideInBytes, (void*)offset);
		glEnableVertexAttribArray(attr.index);

		GLint typeSize = (attr.type == GL_INT) ? sizeof(int) : sizeof(float);
		offset += attr.size * typeSize;
	}

	glBindVertexArray(0);
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}