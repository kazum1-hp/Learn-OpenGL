#define STBI_NO_STDIO   // forbide stb_image use fopen
#include "../external/stb_image/stb_image.h"

#include "../head/Texture.h"
#include <iostream>
#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h> // for MultiByteToWideChar
#include <fstream>
#include <vector>

Texture::Texture(const std::string& path, TextureType typeName)
	: path(path), type(typeName)
{
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Flip upside down
	//stbi_set_flip_vertically_on_load(true);

	// UTF-8 -> UTF-16
	int wlen = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
	std::wstring wpath(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &wpath[0], wlen);
	std::wcout.imbue(std::locale(""));
	//std::wcout << L"[Texture] Trying to load: " << wpath << std::endl;

	// read file to memory by using ifstream
	std::ifstream file(wpath, std::ios::binary | std::ios::ate);
	if (!file) {
		std::cout << "Failed to open texture file: " << path << std::endl;
		return;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<unsigned char> buffer(size);
	if (!file.read((char*)buffer.data(), size)) {
		std::cout << "Failed to read texture file: " << path << std::endl;
		return;
	}

	GLenum internalformat, format;
	// Number of Width, height and Color channels
	int  width, height, nrChannels;

	// load from memory
	unsigned char* data = stbi_load_from_memory(buffer.data(), (int)size, &width, &height, &nrChannels, 0);

	/*unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);*/

	if (data)
	{
		if (nrChannels == 3)
		{
			if (typeName == Diffuse) { internalformat = GL_SRGB; }
			else internalformat = GL_RGB;
			format = GL_RGB;

			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}

		else if (nrChannels == 4)
		{
			if (typeName == Diffuse) { internalformat = GL_SRGB_ALPHA; }
			else internalformat = GL_RGBA;
			format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		std::cerr << "Loading texture: " << path << " ID: " << ID << "texType: " << typeName << std::endl;
		//std::cerr << "Size: " << width << "x" << height << ", Channels: " << nrChannels << std::endl;
	}

	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::bind(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}