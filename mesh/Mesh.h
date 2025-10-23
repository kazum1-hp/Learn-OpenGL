#pragma once

#include "Geometry.h"
#include "Texture.h"
#include "Shader.h"

#include <memory>

class Mesh
{
public:
	Mesh(const Geometry& geometry, const std::vector<std::shared_ptr<Texture>>& textures);
	~Mesh();

	void draw() const;
	
	GLsizei getIndexCount() { return indexCount; }
	std::vector<std::shared_ptr<Texture>> textures;

	GLuint getVAO() const { return VAO; }

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};

