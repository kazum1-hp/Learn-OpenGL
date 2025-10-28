#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
public:
	unsigned int ID;
	int unit;

	Texture(const std::string& path, int textureUnit = 0);
	void bind() const;
	~Texture();
};