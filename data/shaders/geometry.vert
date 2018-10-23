#version 330 core

in vec3 position;
in vec3 texcoord;
in vec3 normal;

out vec3 Normal;
out vec3 Texcoord;
out vec3 Position;

uniform mat4 m;
uniform mat4 vp;

void main(void)
{
        Normal = (m * vec4(normal, 0.0f)).xyz;
        Position = (m * vec4(position, 1.0f)).xyz;
        Texcoord = texcoord;

        mat4 mvp = vp*m;
        gl_Position = mvp*vec4(position, 1.0f);
}
