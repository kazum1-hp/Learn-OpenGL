#include "Mesh.h"
#include <iostream>


Mesh::Mesh(const Geometry& geometry, const std::vector<std::shared_ptr<Texture>>& textures)
	:textures(textures)
{
	const auto& vertices = geometry.vertices;
	const auto& indices = geometry.indices;
	const auto& attributes = geometry.attributes;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	int strideInFloats = 0;
	size_t strideInBytes = 0;
	for (const auto& attr : attributes)
	{
		strideInFloats += attr.size;
		strideInBytes += attr.size * ((attr.type == GL_INT) ? sizeof(int) : sizeof(float));
	}

	size_t offset = 0;
	for (const auto& attr : attributes)
	{
		glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, (GLsizei)strideInBytes, reinterpret_cast<void*>(offset));
		glEnableVertexAttribArray(attr.index);

		GLint typeSize = (attr.type == GL_INT) ? sizeof(int) : sizeof(float);
		offset += attr.size * typeSize;
	}

	indexCount = static_cast<GLsizei>(indices.size());
	glBindVertexArray(0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) std::cout << "GL error after mesh setup: " << err << "\n";

	//std::cout << "Mesh VAO=" << VAO
	//	<< " indices=" << indexCount
	//	<< " vertices=" << vertices.size() << std::endl;

}

void Mesh::draw() const
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		textures[i]->bind();	
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}