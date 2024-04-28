#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

#include <iostream>
#include <vector>

unsigned int texture_from_file(const std::string& path, const std::string& directory);

class Model
{
public:
    Model(const std::string& path);

    void draw(Shader& shader);

private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void load_model(const std::string& path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* material, aiTextureType type, const std::string& type_name);
};

#endif
