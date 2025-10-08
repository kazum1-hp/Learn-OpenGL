#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
};

class Mesh
{
public:
	Mesh(const std::vector<float>& vertices,
		 const std::vector<VertexAttribute>& attributes);
	~Mesh();

	void draw() const;
	
private:
	GLuint VAO, VBO;
	GLsizei vertexCount;
};

