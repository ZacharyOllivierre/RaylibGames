#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

// Self-contained fisheye settings
const float strength = -0.0835;  // positive = barrel/fisheye, negative = pincushion
const vec2 center = vec2(0.5, 0.5); // center of distortion

void main()
{
    // Normalize coordinates to [-1,1] with respect to center
    vec2 uv = fragTexCoord - center;
    uv *= 2.0;

    float r = length(uv);

    // Apply fisheye distortion
    if (r > 0.0)
    {
        float theta = atan(uv.y, uv.x);
        float radius = pow(r, 1.0 - strength);
        uv.x = radius * cos(theta);
        uv.y = radius * sin(theta);
    }

    // Map back to [0,1] range
    uv = uv * 0.5 + center;

    // Sample texture
    finalColor = texture(texture0, uv);
}

