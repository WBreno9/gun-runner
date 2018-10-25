#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 Position;
out vec2 Texcoord;
out vec3 Normal;

layout(std140) uniform matrixBlock {
        mat4 model;
        mat4 view;
        mat4 projection;
}matrices;

void main(void)
{
        Normal = (matrices.model * vec4(normal, 0.0f)).xyz;
        Position = (matrices.model * vec4(position, 1.0f)).xyz;
        Texcoord = texcoord;

        mat4 mvp = matrices.projection * matrices.view * matrices.model;
        gl_Position = mvp*vec4(position, 1.0f);
}
