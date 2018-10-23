#version 330 core

in vec3 Normal;
in vec3 Texcoord;
in vec3 Position;

out vec4 outColor;

void main(void)
{
        outColor = vec4((Normal+0.5f)/2, 1.0f);
}
