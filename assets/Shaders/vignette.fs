#version 330 core

in vec2 fragTexCoord;
out vec4 FragColor;

uniform sampler2D image;
uniform float vignetteRadius;
uniform float vignetteSoftness;
uniform vec3 vignetteColor;

void main() {
    vec2 uv = fragTexCoord * 2.0 - 1.0; // Convert to range [-1, 1]
    float dist = length(uv);
    
    // Calculate vignette effect
    float vignette = smoothstep(vignetteRadius, vignetteRadius - vignetteSoftness, dist);
    
    // Get the original color
    vec4 color = texture(image, fragTexCoord);
    
    // Apply vignette effect
    color.rgb = mix(color.rgb, vignetteColor, vignette);
    
    FragColor = color;
}