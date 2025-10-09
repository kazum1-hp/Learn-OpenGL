#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
public:
	unsigned int ID;

	Texture(const std::string& path, GLenum textureUnit = GL_TEXTURE0);
	/*void bind() const;*/
};