#version 330 core

in vec2 fragTexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform float time; // Time value to animate the glow effect
uniform float edge_from = 0.01;
uniform float edge_to = 0.5;

void main()
{
    vec4 texColor = texture(texture1, fragTexCoord);

    if (texColor.a == 1.0) {
        vec2 center = vec2(0.5, 0.5); // Center of the texture
        float distance = length(fragTexCoord - center); // Distance from the center
        float glow = 0.5 + 0.5 * sin(time * 5.0); // Glow effect

        float edgeDistance = smoothstep(edge_from, edge_to, distance); // Edge distance for glow
        vec4 glowColor = vec4(0.0, 1.0, 0.0, 1.0) * glow * edgeDistance; // Green glow color

        FragColor = texColor + glowColor; // Apply glow effect
    } else {
        FragColor = texColor;
    }
}