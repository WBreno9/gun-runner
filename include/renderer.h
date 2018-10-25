#ifndef RENDERER_H
#define RENDERER_H

#include <inc.h>
#include <model.h>
#include <inputglsl.h>

#include <map>
#include <vector>
#include <list>

struct RVertexAttrib {
        std::string m_name;
        GLuint m_location;
        GLenum m_type;
        unsigned m_size;
        unsigned m_count;

        RVertexAttrib (std::string n, unsigned sz, unsigned cnt, GLenum tp) : 
                m_name(n), m_location(0), m_size(sz), m_count(cnt), m_type(tp) {}
};

struct RUniformBuffer {
        std::string m_name;
        GLuint m_id;

        bool m_isInitialized;
        unsigned m_size;

        RUniformBuffer(std::string n);
        ~RUniformBuffer();

        void setup(unsigned m_size, void* data);
        void update(unsigned offset, unsigned m_size, void* data);
};

struct RShader {
        GLuint m_id;
        std::string m_name;

        std::vector<RUniformBuffer*> m_uniformBuffers;

        RShader(std::string RShaderm_name);
        ~RShader();

        void bind() const;
        void addUniformBuffer(RUniformBuffer* uniformBuffer);
};

struct RVertexBuffer {
        std::string m_name;

        bool m_isInitialized;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;

        std::map<std::string, unsigned> m_attribNames;
        std::vector<RVertexAttrib> m_attribs;
        unsigned m_attribCounter = 0;

        RVertexBuffer(std::string n);
        ~RVertexBuffer();

        void setup(void* vboData, size_t vboSize, GLushort* indices, size_t indicesSize);

        void enableAttribs();
        void addAttrib(RVertexAttrib attrib);
        RVertexAttrib getAttrib(std::string name) const;

        void bind() const;
};

constexpr unsigned numMaxLights = 1000  ;
struct RLight {
        unsigned m_bufferIndex;
        bool m_isActive;
        bool m_modified;

        glm::vec3 m_pos;
        glm::vec3 m_color;

        RLight(glm::vec3 pos, glm::vec3 color) : 
                m_pos(pos), m_color(color), m_isActive(true), m_modified(true) {}

        void setPos(glm::vec3 pos) {
                m_pos = pos;
                m_modified = true;
        }

        void setColor(glm::vec3 color) {
                m_color = color;
                m_modified = true;
        }
};

class Renderer {
public:
        Renderer();
        ~Renderer();

        void loadAllModels();
        void loadModel(const std::string &name);
        ModelHandle findModel(const std::string &modelName);
        void createModelsVBO();

        const std::map<std::string, ModelHandle>* getModelMap();

        void bindModelVBO();

        void drawModel(ModelHandle modelHandle, const glm::mat4 &m, const glm::mat4 &v, const glm::mat4 &p);

        RLight* addLight(RLight light);
        void updateLights();
        void destroyInactiveLights();
private:
        std::map<std::string, ModelHandle> m_modelMap;
        std::vector<Model> m_models;

        RVertexBuffer m_modelBuffer;

        RUniformBuffer m_matricesBuffer;
        RUniformBuffer m_lightsBuffer;

        RShader m_defaultModelShader;

        std::list<RLight> m_lights;
};

#endif