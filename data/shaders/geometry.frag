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

float Lambert(vec3 l, vec3 n){
	float ldotn = clamp(dot(l, n), 0, 1);
	return ldotn;
}

float BlinnPhong(vec3 n, vec3 h, float ex){
	float ndoth = clamp(dot(n, h), 0, 1);
	return pow(ndoth, ex);
}

float SchlickFresnel(vec3 h, vec3 v, float F0){
	return F0 + (1.0-F0) * pow( 1.0 - dot(h, v), 5.0);
}

void main(void)
{
        vec3 albedoColor = vec3(1.0);
        vec3 color = vec3(0);
        vec3 diffuse = vec3(0.0);
        
        for (int i = 0; i < lightsData.numLights; i++) {
                light l = lightsData.lights[i];
                vec3 lpos = l.pos;

                vec3 ldir = normalize(lpos - Position);

                float lightDist = length(lpos - Position);

                float ndotl = dot(Normal, ldir);
                float halfLambert = pow((Lambert(ldir, Normal)*0.5)+0.5, 2);

                diffuse += albedoColor * 500 * l.color * halfLambert * Lambert(ldir, Normal)/(lightDist*lightDist); 
        }

       

        outColor = vec4(diffuse, 1.0f);
}
