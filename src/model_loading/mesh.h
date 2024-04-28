#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

    void draw(Shader& shader);

private:
    // render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void setup_mesh();
};

#endif
