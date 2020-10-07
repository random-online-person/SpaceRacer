#version 330 core
#define NUM_POINT_LIGHTS 1

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 color;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 objectpos;

uniform vec3 cameraPos;

uniform Material material;
uniform Light light[NUM_POINT_LIGHTS];

uniform sampler2D texture_diffuse1;

vec3 CalPointLight(Light light, vec3 Normal, vec3 objectpos, vec3 camerapos) {

	vec3 ambient = material.ambient * light.color;

	vec3 nor = normalize(Normal);
	vec3 LightDir = normalize(light.position - objectpos);
	float diff = max(dot(nor, LightDir), 0.0f);
	vec3 diffuse = diff * light.color * material.diffuse;

	vec3 cameraDir = normalize(cameraPos + objectpos);
	vec3 reflectDir = reflect(-LightDir, nor);
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.color;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 coloroutput = vec3(0.0);
	
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		coloroutput += CalPointLight(light[i], Normal, objectpos, cameraPos);
	}
					   
	vec4 tempColor = texture(texture_diffuse1, TexCoord);

	//FragColor = texture(texture_diffuse1, TexCoord);
	FragColor = vec4(coloroutput ,1.0f) * tempColor;
}