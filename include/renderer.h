#ifndef RENDERER_H
#define RENDERER_H

#include <inc.h>
#include <model.h>
#include <inputglsl.h>
#include <map>

struct RendererVertexBuffer {
        GLuint shader;

        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        GLuint vpMatrixUniLoc;
        GLuint mMatrixUniLoc;

        ~RendererVertexBuffer()
        {
                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vbo);
                glDeleteBuffers(1, &ebo);

                glDeleteShader(shader);
        }

        void Bind() 
        {
                glUseProgram(shader);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
        }
};

class Renderer
{
public:
        Renderer();
        ~Renderer();

        void LoadAllModels();
        void LoadModel(const std::string &name);
        ModelHandle FindModel(const std::string &modelName);
        void CreateModelsVBO();

        inline const std::map<std::string, ModelHandle>* GetModelMap()
        {
                return &modelMap;
        }

        inline void BindModelVBO()
        {
                modelBuffer.Bind();
        }

        inline void DrawModel(ModelHandle modelHandle, const glm::mat4 &m, const glm::mat4 &vp)
        {
                auto model = models[modelHandle];
                auto meshes = model.GetMeshes();

                glUniformMatrix4fv(modelBuffer.mMatrixUniLoc, 1, GL_FALSE, glm::value_ptr(m));
                glUniformMatrix4fv(modelBuffer.vpMatrixUniLoc, 1, GL_FALSE, glm::value_ptr(vp));

                for (auto& mesh : *meshes) {
                        glDrawElementsBaseVertex(
                                GL_TRIANGLES, 
                                mesh.indices.size(),
                                GL_UNSIGNED_SHORT,
                                (void*)(mesh.indexOffset * sizeof(GLushort)),
                                mesh.vertOffset);
                }
        }
private:
        std::map<std::string, ModelHandle> modelMap;
        std::vector<Model> models;

        RendererVertexBuffer modelBuffer;
};

#endif
