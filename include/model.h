#ifndef MODEL_H
#define MODEL_H

#include <inc.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct Vertex {
        Vertex(glm::vec3 c, glm::vec3 n, glm::vec2 t) {
                coords = c;
                normal = n;
                texture = t;
        }

        glm::vec3 coords;
        glm::vec3 normal;
        glm::vec2 texture;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<GLushort> indices;

        std::string textureName;
        std::vector<Texture> textures;

        GLuint indexOffset;
        GLuint vertOffset;
};

class Model {
public:
        Model(const std::string &file_name);

        std::string GetPath();

        bool LoadModel(const std::string &file_name);

        std::vector<Mesh>* GetMeshes();
private:
        std::string name;
        std::string path;
        std::string texturePath;
        
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory);
};

#endif
