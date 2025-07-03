#version 460 core
// uses 2 maps: diffuse and specular @see struct Meterial

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
    sampler2D diffuse;
    sampler2D specular;
    float shininess;

    // поддержка цветового материала
    vec3 diffuse_color;
    vec3 specular_color;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

uniform bool use_texture = true;


void main() {

    // ambient
    vec3 ambient;
    vec3 diffuse_color;
    vec3 specular_color;


    if (use_texture) {
        // Используем текстуры
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        diffuse_color = texture(material.diffuse, TexCoords).rgb;
        specular_color = texture(material.specular, TexCoords).rgb;
    } else {
        // Используем цвета материала
        ambient = light.ambient * material.diffuse_color;
        diffuse_color = material.diffuse_color;
        specular_color = material.specular_color;
    }

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuse_color;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specular_color;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}