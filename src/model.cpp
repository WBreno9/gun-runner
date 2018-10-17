#include <model.h>

Model::Model(const std::string &file_name) {
        LoadModel(file_name);
}

std::string Model::GetPath() {
        return path;
}

bool Model::LoadModel(const std::string &file_name) {
        this->path = file_name;

        std::ifstream fin(file_name.c_str() );

        std::vector<Vertex> vertices;

        if(!fin.fail())
                fin.close();
        else {
                std::cerr << "Couldn't open file: " << file_name << std::endl;
                return false;
        }

        Assimp::Importer assimp_importer;
        const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, 
                        aiProcess_Triangulate | aiProcess_GenSmoothNormals
                        | aiProcess_PreTransformVertices);

        if(!assimp_scene_) {
                std::cerr << assimp_importer.GetErrorString() << std::endl;
                return true;
        }

        if(assimp_scene_->HasMeshes()) {
                const aiVector3D zero(0.0f,0.0f,0.0f);

                //Para cada modelo pegue um ponteiro para esse modelo
                for(uint32_t mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++) {
                        Mesh mesh;

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

                                glm::vec3 coords( vertex_ptr->x, vertex_ptr->y, vertex_ptr->z );
                                glm::vec3 normal( normal_ptr->x, normal_ptr->y, normal_ptr->z );
                                glm::vec2 texture (texture_ptr->x, texture_ptr->y );

                                Vertex v(coords, normal, texture);

                                mesh.vertices.push_back(v);
                        }

                        for (unsigned i = 0; i < mesh_ptr->mNumFaces; i++) {
                                aiFace face = mesh_ptr->mFaces[i];
        
                                for (unsigned j = 0; j < face.mNumIndices; j++)
                                        mesh.indices.push_back(face.mIndices[j]);
                        }

                        meshes.push_back(mesh);
                }
        }

        return true;
}

std::vector<Mesh>* Model::GetMeshes()
{
        return &meshes;
}
