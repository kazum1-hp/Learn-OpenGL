#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, 
		   /*const std::vector<unsigned int>& indices,*/
		   const std::vector<VertexAttribute>& attributes)
{
	//indexCount = static_cast<GLsizei>(indices.size());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	/*glGenBuffers(1, &EBO);*/

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);*/

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
	/*glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);*/
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	/*glDeleteBuffers(1, &EBO);*/
}