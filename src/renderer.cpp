#include <renderer.h>

Renderer::Renderer()
{
        modelBuffer.shader = GLSLShaderProgram("data/shaders/geometry.vert", "data/shaders/geometry.frag");
        LoadAllModels();
        CreateModelsVBO();
}

Renderer::~Renderer()
{
}

void Renderer::LoadAllModels()
{
        std::clog << "Loading models" << std::endl;
        LoadModel("error");
        LoadModel("test");
        LoadModel("building");

}

void Renderer::LoadModel(const std::string &modelName)
{
        std::clog << "  Model: " + modelName << std::endl;

        std::string fileName = "data/models/" + modelName + ".fbx";
        Model model(fileName);
        models.push_back(model);

        modelMap.emplace(std::make_pair(modelName, models.size()-1));
}

void Renderer::CreateModelsVBO()
{
        std::vector<Vertex> allVertices;
        std::vector<GLushort> allIndices;

        size_t meshCount = 0;

        for (auto& model : models) {
                auto meshes = model.GetMeshes();
                for (auto& mesh : *meshes) {
                        mesh.indexOffset = allIndices.size();
                        mesh.vertOffset = allVertices.size();

                        allVertices.insert(allVertices.end(), mesh.vertices.begin(), mesh.vertices.end());
                        allIndices.insert(allIndices.end(), mesh.indices.begin(), mesh.indices.end());

                        meshCount++;
                }
        }


        std::clog << "Mesh Count: "
                  << meshCount << std::endl;

        std::clog << "Models total vertex count: " << allVertices.size() << "\n"
                  << "Models total indices count: " << allIndices.size()
                  << std::endl;

        std::clog << "Creating Models VAO" << std::endl;

        glGenVertexArrays(1, &modelBuffer.vao);
        glBindVertexArray(modelBuffer.vao);

        std::clog << "Creating Models VBO";

        GLsizeiptr vertexArrayBufferSize = allVertices.size()*sizeof(Vertex);
        std::clog << " Size: " << vertexArrayBufferSize << std::endl;

        glGenBuffers(1, &modelBuffer.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, modelBuffer.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexArrayBufferSize, &allVertices[0], GL_STATIC_DRAW);

        std::clog << "Creating Models EBO";

        GLsizeiptr elementArrayBufferSize = allIndices.size()*sizeof(GLushort);
        std::clog << " Size: " << elementArrayBufferSize << std::endl;

        glGenBuffers(1, &modelBuffer.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelBuffer.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferSize, &allIndices[0], GL_STATIC_DRAW);
        
        std::clog << "Setting vertex attributes" << std::endl;

        GLuint posAttrib = glGetAttribLocation(modelBuffer.shader, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);

        GLuint normAttrib = glGetAttribLocation(modelBuffer.shader, "normal");
        glEnableVertexAttribArray(normAttrib);
        glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

        GLuint texAttrib = glGetAttribLocation(modelBuffer.shader, "texcoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::clog << "Getting uniforms" << std::endl;

        modelBuffer.vpMatrixUniLoc = glGetUniformLocation(modelBuffer.shader, "vp");
        modelBuffer.mMatrixUniLoc = glGetUniformLocation(modelBuffer.shader, "m");
}

ModelHandle Renderer::FindModel(const std::string &modelName)
{
        auto result = modelMap.find(modelName);
        if (result != modelMap.end()) {
                return result->second;
        } else {
                std::clog << "Failed to find Model: " + modelName
                          << std::endl;
                return 0;
        }
}
