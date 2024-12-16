#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    float shininess;
};

uniform Material material;
uniform vec3 viewPos;
uniform float time;

out vec4 FragColor;

// Underwater effect parameters
const vec3 WATER_COLOR = vec3(0.5, 0.6, 0.9);
const float WATER_DEPTH = 20.0;

void main() {
    // Normalize
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Texture sampling with slight distortion
    vec2 distortedTexCoords = TexCoords + 
        vec2(sin(time * 0.1), cos(time * 0.1)) * 0.01;
    
    vec4 texColor1 = texture(material.texture_diffuse1, distortedTexCoords);
    vec4 texColor2 = texture(material.texture_diffuse2, distortedTexCoords);
    
    // Blend textures
    vec4 modelColor = mix(texColor1, texColor2, 0.5);

    // Dynamic light directions
    vec3 lightDirs[3] = vec3[3](
        normalize(vec3(sin(time), 1.0 + cos(time * 0.5), cos(time))),
        normalize(vec3(sin(time * 1.2), 1.0 + sin(time * 0.3), cos(time * 0.8))),
        normalize(vec3(0.0, 1.0 + sin(time * 0.2), 0.0))
    );

    // Lighting calculations
    vec3 ambient = WATER_COLOR * 0.3;
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    float reducedShininess = material.shininess * 0.3;

    for(int i = 0; i < 3; i++) {
        // Diffuse
        float diff = max(dot(normal, lightDirs[i]), 0.0);
        totalDiffuse += diff * WATER_COLOR * 0.5;

        // Specular
        vec3 halfwayDir = normalize(lightDirs[i] + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), reducedShininess);
        totalSpecular += spec * vec3(0.5, 0.6, 0.7) * 0.2;
    }

    // Depth calculations
    float depth = length(FragPos);
    vec3 depthAbsorption = exp(-depth / WATER_DEPTH * WATER_COLOR);

    // Combine lighting
    vec3 lighting = ambient + totalDiffuse + totalSpecular;

    // Final color composition
    // Blend original texture with underwater color effect
    vec3 underwaterEffect = WATER_COLOR * (0.2 + 0.1 * sin(time));
    vec3 finalColor = modelColor.rgb * lighting * (1.0 + underwaterEffect);

    // Subtle depth-based color modification
    finalColor = mix(finalColor, finalColor * depthAbsorption, 0.3);

    FragColor = vec4(finalColor, modelColor.a);
}