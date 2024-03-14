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
	void set_bool(std::string name, bool value) const;
	void set_int(std::string name, int value) const;
	void set_float(std::string name, float value) const;
    void set_matrix(std::string name, glm::mat4 value) const;
};

#endif
