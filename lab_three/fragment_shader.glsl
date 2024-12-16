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

// Advanced underwater effect parameters
const vec3 DEEP_WATER_COLOR = vec3(0.1, 0.3, 0.6);
const vec3 SHALLOW_WATER_COLOR = vec3(0.5, 0.7, 1.0);
const float WATER_DEPTH = 20.0;
const float CAUSTIC_INTENSITY = 0.9;

// Noise function for caustics and distortion
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

// Caustics simulation
float caustics(vec2 uv, float time) {
    vec2 movement = vec2(sin(time * 0.2), cos(time * 0.3)) * 0.1;
    float caustic1 = sin(uv.x * 20.0 + time * 2.0 + movement.x);
    float caustic2 = cos(uv.y * 15.0 - time * 1.5 + movement.y);
    return (caustic1 * caustic2 + 1.0) * 0.5;
}

void main() {
    //Texture distortion
    vec2 distortedTexCoords = TexCoords + vec2(
        sin(time * 0.2 + TexCoords.y * 10.0) * 0.02,
        cos(time * 0.15 + TexCoords.x * 10.0) * 0.02
    );
    
    // Texture sampling with additional variation
    vec4 texColor1 = texture(material.texture_diffuse1, distortedTexCoords);
    vec4 texColor2 = texture(material.texture_diffuse2, distortedTexCoords);
    
    // Blending textures with additional noise
    float noiseValue = noise(distortedTexCoords * 10.0);
    vec4 modelColor = mix(texColor1, texColor2, 0.5 + noiseValue * 0.2);

    // Normalized vectors
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Dynamic, organic light movement
    vec3 lightDirs[3] = vec3[3](
        normalize(vec3(sin(time * 1.1), 1.0 + cos(time * 0.7), cos(time * 1.3))),
        normalize(vec3(sin(time * 0.9), 1.0 + sin(time * 0.5), cos(time * 1.1))),
        normalize(vec3(0.0, 1.0 + sin(time * 0.4), cos(time * 0.6)))
    );

    // Ambient lighting with underwater color gradient
    vec3 ambient = mix(DEEP_WATER_COLOR, SHALLOW_WATER_COLOR, 0.5) * 0.2;

    // Advanced lighting calculations
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    float reducedShininess = material.shininess * 0.15;

    for(int i = 0; i < 3; i++) {
        // Soft diffuse lighting
        float diff = pow(max(dot(normal, lightDirs[i]), 0.0), 1.5);
        totalDiffuse += diff * mix(DEEP_WATER_COLOR, SHALLOW_WATER_COLOR, diff) * 0.4;

        // Soft specular highlights
        vec3 halfwayDir = normalize(lightDirs[i] + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), reducedShininess);
        totalSpecular += spec * vec3(0.6, 0.7, 0.8) * 0.3;
    }

    // Depth and absorption calculations
    float depth = length(FragPos);
    vec3 depthAbsorption = exp(-depth / WATER_DEPTH * DEEP_WATER_COLOR);

    // Caustics effect
    float causticEffect = caustics(distortedTexCoords, time) * CAUSTIC_INTENSITY;

    // Lighting composition
    vec3 lighting = ambient + totalDiffuse + totalSpecular;

    // Final color with underwater characteristics
    vec3 underwaterEffect = mix(DEEP_WATER_COLOR, SHALLOW_WATER_COLOR, sin(time * 0.5) * 0.5 + 0.5);
    vec3 finalColor = modelColor.rgb * lighting * (1.0 + underwaterEffect * 0.2);

    // Apply depth absorption, caustics, and final color blending
    finalColor = finalColor * depthAbsorption * (1.0 + causticEffect);

    // Optional: add a subtle color shift based on depth
    finalColor = mix(finalColor, underwaterEffect, depth / WATER_DEPTH * 0.3);

    FragColor = vec4(finalColor, modelColor.a);
}