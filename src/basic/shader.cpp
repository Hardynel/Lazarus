#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	// retrieve vertex/fragment source code from filePath
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream vertex_shader_file;
	std::ifstream fragment_shader_file;

	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);

		std::stringstream vertex_shader_stream;
		std::stringstream fragment_shader_stream;

		// read file's buffer contents into streams
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		// converts stream into string
		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();
	}
	catch (std::ifstream::failure& ex)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:" << ex.what() << std::endl;
	}

	const char* vertex_shader_source = vertex_code.c_str();
	const char* fragment_shader_source = fragment_code.c_str();

	unsigned int vertex;
	unsigned int fragment;

	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILE_ERROR\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILE_ERROR\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINK_ERROR\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::set_bool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::set_int(const std::string name&, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string name&, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_matrix(const std::string name&, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
