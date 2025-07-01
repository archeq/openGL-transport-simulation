#version 460 core
/*
    used for drawing models loaded via assimp @see Model
*/

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


out vec4 color;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D specular1;
    sampler2D specular2;
    sampler2D specular3;
    float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main() {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}