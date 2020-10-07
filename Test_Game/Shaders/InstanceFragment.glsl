#version 330 core

out vec4 FragColor;

struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 color;
};


in vec3 ObjPos;
in vec3 Normal; 
in vec2 TexCoords;


uniform sampler2D texture_diffuse1;

uniform vec3 cameraPos;

uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = light.ambient;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - ObjPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;

	// specular
	vec3 viewDir = normalize(cameraPos - ObjPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec;

	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction));
	//float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - ObjPos);
	distance /= 500.f;
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	result *= light.color;
	FragColor = vec4(result * texture(texture_diffuse1, TexCoords).rgb, 1.0);
}