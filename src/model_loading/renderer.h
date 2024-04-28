#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "shader.h"

class Renderer 
{
public:
    Renderer(const std::vector<float>& vertices);

    void draw(Shader& shader);

private:
    std::vector<float> vertices;

    unsigned int VAO;
    unsigned int VBO;

    void setup_renderer();
};

#endif
