#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>


class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	template<typename T>
	void setUniform(const std::string& name, const T& value) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

template<>
inline void Shader::setUniform<int>(const std::string& name, const int& value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

template<>
inline void Shader::setUniform<float>(const std::string& name, const float& value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

template<>
inline void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

template<>
inline void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
