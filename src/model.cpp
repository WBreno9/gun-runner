#include <model.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Model::Model(const std::string &file_name) {
    LoadModel(file_name);
}

std::string Model::GetPath() {
    return path;
}

bool Model::LoadModel(const std::string &file_name) {
    this->path = file_name;

    std::ifstream fin(file_name.c_str() );

    if(!fin.fail()){
        fin.close();
    } else {
        std::cerr << "Couldn't open file: " << file_name << std::endl;
        return false;
    }
    
    Assimp::Importer assimp_importer;
    const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
    
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
                glm::vec3 coords(vertex_ptr->x, vertex_ptr->y, vertex_ptr->z);
                glm::vec3 normal(normal_ptr->x, normal_ptr->y, normal_ptr->z);
                glm::vec2 texture (texture_ptr->x, texture_ptr->y);
                Vertex v(coords, normal, texture);
                mesh.vertices.push_back(v);
            }
            for (unsigned i = 0; i < mesh_ptr->mNumFaces; i++) {
                aiFace face = mesh_ptr->mFaces[i];
                for (unsigned j = 0; j < face.mNumIndices; j++){
                    mesh.indices.push_back(face.mIndices[j]);
                }
            }
            //Processamento de texturas
            aiMaterial* material = assimp_scene_->mMaterials[mesh_ptr->mMaterialIndex];
            //Mapa difuso
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            mesh.textures.insert(mesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            //Mapa especular
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            mesh.textures.insert(mesh.textures.end(), specularMaps.begin(), specularMaps.end());
            //Mapa de normais
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            mesh.textures.insert(mesh.textures.end(), normalMaps.begin(), normalMaps.end());
            //Mapa de alturas
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            mesh.textures.insert(mesh.textures.end(), heightMaps.begin(), heightMaps.end());
            
            meshes.push_back(mesh);
        }
    }
    return true;
}

std::vector<Mesh>* Model::GetMeshes() {
    return &meshes;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++){
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), texturePath);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data){
        GLenum format;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        //glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}