#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	// uniform functions
	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
    void set_matrix(const std::string& name, const glm::mat4& value) const;
    void set_vec3(const std::string& name, float x, float y, float z) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;

};

#endif
