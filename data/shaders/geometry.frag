#version 330 core

in vec3 Position;
in vec2 Texcoord;
in vec3 Normal;

out vec4 outColor;

#define NUM_MAX_LIGHTS 1000
struct light {
        vec3 pos;
        vec3 color;
};

layout(std140) uniform lightsBlock {
        int numLights;
        light lights[NUM_MAX_LIGHTS];
}lightsData;

void main(void)
{
        vec3 diffColor = vec3(1.0f);
        vec3 color = vec3(0.f);

        for (int i = 0; i < lightsData.numLights; i++) {
                light l = lightsData.lights[i];

                vec3 ldir = normalize(l.pos - Position);
                float dist = length(l.pos - Position);
                float attenuation = 1.0f / (dist * dist);

                float ndotl = dot(Normal, ldir);

                color += diffColor * vec3(ndotl) * attenuation * l.color;   
        }

        outColor = vec4(color, 1.0f);
}
