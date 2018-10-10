#include <model.h>

Mesh::Mesh(const std::string &file_name) {
        loadMesh(file_name);
}

std::string Mesh::getPath() {
        return path;
}

void Mesh::loadMesh(const std::string &file_name) {
        this->path = file_name;

        std::ifstream fin(file_name.c_str() );

        std::vector<Vertex> vertices;

        if(!fin.fail())
                fin.close();
        else {
                std::cerr << "Couldn't open file: " << file_name << std::endl;
                return;
        }

        Assimp::Importer assimp_importer;
        const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, 
                        aiProcess_Triangulate | aiProcess_GenSmoothNormals);

        if(!assimp_scene_) {
                std::cerr << assimp_importer.GetErrorString() << std::endl;
                return;
        }

        if(assimp_scene_->HasMeshes()) {
                const aiVector3D zero(0.0f,0.0f,0.0f);

                //Para cada modelo pegue um ponteiro para esse modelo
                for(uint32_t mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++) {
                        const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];
                        bool hasTexture = mesh_ptr->HasTextureCoords(0);

                        //para cada vertice do modelo pegue um ponteiro para esse vertice, 
                        //sua normal e suas coordenadas de textura
                        for(uint32_t vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id++) {
                                const aiVector3D *vertex_ptr = &(mesh_ptr->mVertices[vertex_id]);
                                const aiVector3D *normal_ptr = &(mesh_ptr->mNormals[vertex_id]);
                                const aiVector3D *texture_ptr;
                                if(hasTexture) {
                                        texture_ptr = &(mesh_ptr->mTextureCoords[0][vertex_id]);    
                                } else {
                                        texture_ptr = &zero;
                                }

                                glm::vec4 coords( vertex_ptr->x, vertex_ptr->y, vertex_ptr->z,1 );
                                glm::vec3 normal( normal_ptr->x, normal_ptr->y, normal_ptr->z );
                                glm::vec2 texture (texture_ptr->x, texture_ptr->y );

                                Vertex v(coords, normal, texture);

                                vertices.push_back(v);
                        }
                }
        }

        this->vertices = vertices;
}
