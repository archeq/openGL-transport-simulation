
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

struct MaterialColor {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform MaterialColor materialColor;
uniform Light light;
uniform bool useTextures;

void main() {
    vec3 color;
    vec3 specularColor;
    vec3 normal;

    if (useTextures) {
        // Sample normal from normal map
        normal = texture(material.normal1, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);

        // Get diffuse color from texture
        color = texture(material.diffuse1, TexCoords).rgb;
        specularColor = texture(material.specular1, TexCoords).rgb;
    } else {
        // Use material colors
        color = materialColor.diffuse;
        specularColor = materialColor.specular;
        // Use vertex normal (transformed to tangent space would be better, but this works)
        normal = vec3(0.0, 0.0, 1.0); // Default normal in tangent space
    }

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

    float shininess = useTextures ? material.shininess : materialColor.shininess;
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec3 specular = specularColor * spec * light.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}