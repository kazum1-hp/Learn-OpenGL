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
	
	GLsizei getIndexCount() const { return indexCount; }
	std::vector<std::shared_ptr<Texture>> textures;

	GLuint getVAO() const { return VAO; }
	void addTexture(const std::shared_ptr<Texture>& texture) {
		textures.push_back(texture);
	}


private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};

