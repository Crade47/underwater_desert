#version 330 core


// in vec3 Normal;
in vec2 TexCoords;


struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;

};


struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 position;
    vec3 lightColor;
    vec3 lightSource;
};

uniform Material material;


Light light;
vec3 viewPos;

out vec4 FragColor;

void main() {
    //AMBIENT LIGHTING
    light.ambient = vec3(0.5, 0.5, 0.5);
    

    //DIFFUSE LIGHTING
    vec3 Normal = vec3(1.0,0.0,0.0);
    vec3 normal = normalize(Normal);
    light.lightColor = vec3(0.0, 0.0, 0.5);
    light.lightSource = vec3(1.0, 0.0, 0.0);
    float diffuseStrength = max(dot(light.lightSource, normal), 0.0);
    light.diffuse = diffuseStrength * light.lightColor;

    //TEXTURE STUFF
    vec4 texColor1 = texture(material.texture_diffuse1, TexCoords);
    vec4 texColor2 = texture(material.texture_diffuse2, TexCoords);
    vec4 modelColor = mix(texColor1, texColor2, 0.9);

    //COMBINE LIGHTING
    vec3 lighting = light.ambient + light.diffuse;

    FragColor = vec4(modelColor.rgb * lighting, modelColor.a);
}