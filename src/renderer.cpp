#include <renderer.h>

RUniformBuffer::RUniformBuffer(std::string n) : m_name(n), m_isInitialized(false) {   
}

RUniformBuffer::~RUniformBuffer() {
        if (m_isInitialized) {
                glDeleteBuffers(1, &m_id);
        }
}

void RUniformBuffer::setup(unsigned size, void* data) {
        m_size = size;

        glGenBuffers(1, &m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferData(GL_UNIFORM_BUFFER, m_size, data, GL_DYNAMIC_DRAW);

        m_isInitialized = true;
}

void RUniformBuffer::update(unsigned offset, unsigned size, void* data) {
        if (size > m_size || (offset + size) > m_size) {
                std::clog << "Failed to update uniform buffer: " << m_name
                        << " unsuficient space" << std::endl;
                return;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

RShader::RShader(std::string shaderName) : m_name(shaderName) {
        std::string filePath = "data/shaders/" + shaderName;
        m_id = GLSLShaderProgram(filePath + ".vert", filePath + ".frag");

        if (!m_id) {
                std::clog << "Failed to create shader: "
                        << shaderName
                        << std::endl;
        }
}

RShader::~RShader() {
        glDeleteShader(m_id);
}

void RShader::bind() const {
        glUseProgram(m_id);

        for (auto& it : m_uniformBuffers) {
                glBindBuffer(GL_UNIFORM_BUFFER, it->m_id);
        }
}

void RShader::addUniformBuffer(RUniformBuffer* uniformBuffer) {
        if (!uniformBuffer->m_isInitialized) {
                std::clog << "Uniform buffer: " << uniformBuffer->m_name
                        << " is not initialized" << std::endl;
                return;
        }

        GLuint bindingPoint = m_uniformBuffers.size();
        GLuint blockIndex = glGetUniformBlockIndex(m_id, uniformBuffer->m_name.c_str());

        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->m_id);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uniformBuffer->m_id);
        glUniformBlockBinding(m_id, blockIndex, bindingPoint);

        m_uniformBuffers.push_back(uniformBuffer);
}

RVertexBuffer::RVertexBuffer(std::string n) : m_name(n) {
}

RVertexBuffer::~RVertexBuffer() {
        if (m_isInitialized) {
                glDeleteVertexArrays(1, &m_vao);
                glDeleteBuffers(1, &m_vbo);
                glDeleteBuffers(1, &m_ebo);
        }
}

void RVertexBuffer::setup(void* vboData, size_t vboSize, GLushort* indices, size_t indicesSize) {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vboSize, vboData, GL_STATIC_DRAW);

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

        m_isInitialized = true;

        enableAttribs();
}

void RVertexBuffer::enableAttribs() {
        unsigned stride = 0;

        for (auto& it : m_attribs) 
                stride += it.m_size * it.m_count;

        uintptr_t offset = 0;
        for (auto& it : m_attribs) {
                if (it.m_count == 0) {
                        std::clog << "Failed to enable vertex attribute: "
                                << it.m_name << " to vertex buffer: "
                                << m_name << std::endl;

                                break;
                }

                glEnableVertexAttribArray(it.m_location);
                glVertexAttribPointer(it.m_location, 
                                        it.m_count, 
                                        it.m_type, 
                                        GL_FALSE, 
                                        stride,
                                        (void*)(offset));

                offset += it.m_size * it.m_count;
        }
}

void RVertexBuffer::bind() const {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void RVertexBuffer::addAttrib(RVertexAttrib attrib) {
        auto res = m_attribNames.find(attrib.m_name);

        if (res != m_attribNames.end()) {
                std::clog << "Failed to add vertex attribute: "
                        << attrib.m_name << " to shader: "
                        << m_name << std::endl;

                return;
        }

        attrib.m_location = m_attribCounter++;

        m_attribNames.emplace(std::make_pair(attrib.m_name, m_attribs.size()));
        m_attribs.push_back(attrib);
}

RVertexAttrib RVertexBuffer::getAttrib(std::string name) const {
        auto result = m_attribNames.find(name);
        if (result != m_attribNames.end()) {
                return m_attribs[result->second];
        } else {
                return RVertexAttrib("ERROR", 0, 0, 0);
        }
}

Renderer::Renderer() : 
        m_defaultModelShader("geometry"), 
        m_modelBuffer("modelsBuffer"), 
        m_matricesBuffer("matrixBlock"),
        m_lightsBuffer("lightsBlock")
        {
        m_matricesBuffer.setup(3*sizeof(glm::mat4), nullptr);
        m_lightsBuffer.setup(numMaxLights * 32 + 16, nullptr);

        unsigned numLights = 0;
        m_lightsBuffer.update(0, sizeof(GLint), &numLights);

        m_defaultModelShader.addUniformBuffer(&m_matricesBuffer);
        m_defaultModelShader.addUniformBuffer(&m_lightsBuffer);

        //glEnable(GL_CULL_FACE);

        loadAllModels();
        createModelsVBO();
}

Renderer::~Renderer() {
}

void Renderer::loadAllModels() {
        std::clog << "Loading models" << std::endl;
        loadModel("error");
        loadModel("test_map");
        loadModel("box");
        loadModel("nanosuit/nanosuit");
}

void Renderer::loadModel(const std::string &modelName) {
        std::clog << "  Model: " + modelName << std::endl;

        std::string fileName = "data/models/" + modelName + ".fbx";
        Model model(fileName);
        m_models.push_back(model);

        m_modelMap.emplace(std::make_pair(modelName, m_models.size()-1));
}

void Renderer::createModelsVBO() {
        std::vector<Vertex> allVertices;
        std::vector<GLushort> allIndices;

        size_t meshCount = 0;

        for (auto& model : m_models) {
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
                  << "Models total indices count: " << allIndices.size() << "\n"
                  << std::endl;

        m_modelBuffer.addAttrib(RVertexAttrib("position", sizeof(GLfloat), 3, GL_FLOAT));
        m_modelBuffer.addAttrib(RVertexAttrib("normal", sizeof(GLfloat), 3, GL_FLOAT));
        m_modelBuffer.addAttrib(RVertexAttrib("texcoord", sizeof(GLfloat), 2, GL_FLOAT));

        m_modelBuffer.setup(&allVertices[0], allVertices.size()*sizeof(Vertex), 
						&allIndices[0], allIndices.size()*sizeof(GLushort));
}

RModelHandle Renderer::findModel(const std::string &modelName) {
        auto result = m_modelMap.find(modelName);
        if (result != m_modelMap.end()) {
                return result->second;
        } else {
                std::clog << "Failed to find Model: " + modelName
                          << std::endl;
                return 0;
        }
}

const std::map<std::string, RModelHandle>* Renderer::getModelMap() {
                return &m_modelMap;
}

void Renderer::bindModelVBO() {
        m_modelBuffer.bind();
        m_defaultModelShader.bind();
}

void Renderer::drawModel(RModelHandle modelHandle, const glm::mat4 &m, const glm::mat4 &v, const glm::mat4 &p)
{
        auto model = m_models[modelHandle];
        auto meshes = model.GetMeshes();

        m_matricesBuffer.update(                  0, sizeof(glm::mat4), (void*)glm::value_ptr(m));
        m_matricesBuffer.update(  sizeof(glm::mat4), sizeof(glm::mat4), (void*)glm::value_ptr(v));
        m_matricesBuffer.update(2*sizeof(glm::mat4), sizeof(glm::mat4), (void*)glm::value_ptr(p));

        for (auto& mesh : *meshes) {
                glDrawElementsBaseVertex(
                        GL_TRIANGLES,
                        mesh.indices.size(),
                        GL_UNSIGNED_SHORT,
                        (void*)(mesh.indexOffset * sizeof(GLushort)),
                        mesh.vertOffset);
        }
}

RLight* Renderer::addLight(RLight light) {
        if (m_lights.size() == numMaxLights) {
                return nullptr;
        }
        
        light.m_bufferIndex = m_lights.size();
        m_lights.push_back(light);

        unsigned numLights = m_lights.size();
        m_lightsBuffer.update(0, sizeof(GLint), &numLights);

        return &m_lights.back();
}

void Renderer::destroyInactiveLights() {
        for (auto it = m_lights.begin(); it != m_lights.end();) {
                if (!it->m_isActive) {
                        auto tmp = it++;
                        m_lights.erase(tmp);
                        unsigned numLights = m_lights.size();
                        m_lightsBuffer.update(0, sizeof(GLint), &numLights);

			if (it == m_lights.end()) 
				break;
		} else {
			it++;
		}
        }
}

void Renderer::updateLights() {
        for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
                if (it->m_isActive && it->m_modified) {
                        glm::vec4 data[2] = {glm::vec4(it->m_pos, 1.0f), glm::vec4(it->m_color, 1.0f)};

                        m_lightsBuffer.update(sizeof(GLfloat) * 4 + (sizeof(GLfloat) * 8 * it->m_bufferIndex), 
                                sizeof(GLfloat) * 8, &data);

                        it->m_modified = false;
                } 
        }
}