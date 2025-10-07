#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	
	//Cache glGetUniformLocation
	mutable std::unordered_map<std::string, GLint> uniformCache;

	GLint getUniformLocation(const std::string& name) const
	{
		if (uniformCache.find(name) != uniformCache.end())
			return uniformCache.at(name);

		GLint location = glGetUniformLocation(ID, name.c_str());
		uniformCache[name] = location;
		return location;
	}
};

template<>
inline void Shader::setUniform<int>(const std::string& name, const int& value) const {
	glUniform1i(getUniformLocation(name), value);
}

template<>
inline void Shader::setUniform<float>(const std::string& name, const float& value) const {
	glUniform1f(getUniformLocation(name), value);
}

template<>
inline void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

template<>
inline void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& value) const {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}
