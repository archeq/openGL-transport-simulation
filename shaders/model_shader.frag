#version 460 core

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {
    // Sample normal from normal map
    vec3 normal = texture(material.normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // Get diffuse color
    vec3 color = texture(material.diffuse1, TexCoords).rgb;
    
    // Ambient
    vec3 ambient = light.ambient * color;
    
    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * color;
    
    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    vec3 specular = vec3(texture(material.specular1, TexCoords)) * spec * light.specular;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}