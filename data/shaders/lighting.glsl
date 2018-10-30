!VERTPROGRAM
#version 120

attribute vec2 position;
attribute vec2 texcoord;

varying vec2 Texcoord;

void main(void)
{
	Texcoord = texcoord;
	gl_Position = vec4(position, 0, 1);
}

!END
!FRAGPROGRAM
#version 120
#define PI 3.1415926535897932384626433832795

uniform sampler2D albedoBuffer;
uniform sampler2D posBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specBuffer;
uniform sampler2D glossBuffer;
uniform sampler2D ZBuffer;

uniform samplerCube cubemap;

varying vec2 Texcoord;

uniform vec3 camPos;

struct light
{
	vec3 pos;
	vec3 color;
	float pow;
};

const int lightSz = 2;
light lights[lightSz];

light light1 = light(
	vec3(4.0, 5.5, -4.0)*vec3(1.5),
	vec3(1.0, 1.0, 1.0),
	32.0
);

light light2 = light(
	vec3(-8.0, 4.5, -2.0)*vec3(1.1),
	vec3(1.0, 1.0, 1.0),
	28.0
);


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

void main() 
{
	vec3 outColor = vec3(0.0);
	
	vec3 albedoColor = texture2D(albedoBuffer, Texcoord).rgb; 
	vec3 n = texture2D(normalBuffer, Texcoord).rgb;
	vec3 position = texture2D(posBuffer, Texcoord).rgb;
	vec3 specularColor = texture2D(specBuffer, Texcoord).rgb;
	float gloss = texture2D(glossBuffer, Texcoord).r;
	float zbuff = texture2D(ZBuffer, Texcoord).r;
	
	float zn = 1.0; 
	float zf = 100.0; 
	float linearz = (2.0 * zn) / (zf + zn - zbuff * (zf - zn));	
	
	lights[0] = light1;
	lights[1] = light2;
	
	vec3 ambient = albedoColor*vec3(0.1, 0.1, 0.1);
	
	float ior = 1.1;
	
	vec3 v = normalize(camPos - position);
		
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	vec3 totalLighting = vec3(0.0);
		
	vec3 r = reflect(-v, n);
	vec3 env1 = textureCube(cubemap, r, 5).rgb;
	vec3 env2 = textureCube(cubemap, r, 6).rgb;
	
	vec3 env = mix(env1, env2, 0.5);
	
	vec3 enva = textureCube(cubemap, r, 7).rgb;
		
	for(int i=0; i<lightSz; i++){
		
		float lightDist = length(lights[i].pos - position);
	
		vec3 l = normalize(lights[i].pos - position);
		
		vec3 h = normalize(l+v);
		
		float f0 = abs((1.0 - ior)/(1.0 + ior));
		f0 = f0*f0;
		f0 = mix(vec3(f0), vec3(1.0), 0.1).x;
		
		float F = SchlickFresnel(n, v, f0);
		
		float Blinn = BlinnPhong(n, h, gloss*200)+(F*0.4);
		
		float halfLambert = pow((Lambert(l, n)*0.5)+0.5, 2);
		
		diffuse += albedoColor * lights[i].pow * lights[i].color * halfLambert * Lambert(l, n)/(lightDist*lightDist);
		specular += 3.0 * specularColor * env  * lights[i].pow * Blinn/(lightDist*lightDist);
	}
	totalLighting = diffuse+specular+ambient+(enva*0.1);
	
	if(linearz < 1.0)
		outColor = vec3(totalLighting);
	else
		outColor = vec3(albedoColor);

	gl_FragColor = vec4(outColor, 1.0);
}
!END

