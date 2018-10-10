#ifndef MODEL_H
#define MODEL_H

#include <inc.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct Vertex {
        Vertex(glm::vec4 c, glm::vec3 n, glm::vec2 t) {
                coords = c;
                normal = n;
                texture = t;
        }

        glm::vec4 coords;
        glm::vec3 normal;
        glm::vec2 texture;
};

class Mesh {
public:
        Mesh(const std::string &file_name);
        std::string getPath();
        void loadMesh(const std::string &file_name);
        const std::vector<Vertex>* GetMeshVertices();

private:
        std::string path;
        std::vector<Vertex> vertices;
};

#endif
