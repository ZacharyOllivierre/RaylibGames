#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

// Bloom settings
uniform float threshold = 0.6;   // Only brighter pixels bloom
uniform float intensity = 2.5;   // Bloom intensity
uniform float blurSize = 2.4;    // Controls blur radius

// Simple Gaussian kernel weights for a 5x5 blur
const float kernel[5] = float[](0.204164, 0.304005, 0.093913, 0.010381, 0.001217);

vec3 getBloom(sampler2D tex, vec2 uv)
{
    vec3 bloom = vec3(0.0);
    vec3 color;
    float lum;

    // Simple 5x5 Gaussian blur
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            vec2 offset = vec2(float(x), float(y)) * blurSize / vec2(textureSize(tex,0));
            color = texture(tex, uv + offset).rgb;

            // Only bloom bright areas
            color = max(color - vec3(threshold), vec3(0.0));

            // Apply kernel weight based on distance
            float weight = kernel[abs(x)] * kernel[abs(y)];
            bloom += color * weight;
        }
    }
    return bloom;
}

void main()
{
    vec3 original = texture(texture0, fragTexCoord).rgb;

    // Add bloom while preserving original color
    vec3 bloom = getBloom(texture0, fragTexCoord) * intensity;

    finalColor = vec4(original + bloom, 1.0);
}

