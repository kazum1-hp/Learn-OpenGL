#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
public:
	unsigned int ID;
	int unit;
	std::string type;

	Texture(const std::string& path, int textureUnit = 0, const std::string& typeName = "diffuse");

	Texture(GLuint textureID, int textureUnit = 0, const std::string& typeName = "color")
		: ID(textureID), unit(textureUnit), type(typeName) {
	}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		:ID(other.ID), unit(other.unit)
	{
		other.ID = 0;
	}

	Texture& operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &ID);
			ID = other.ID;
			unit = other.unit;
			other.ID = 0;
		}
		return *this;
	}

	void bind() const;
	~Texture();
};