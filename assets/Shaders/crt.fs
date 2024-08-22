#version 330 core

in vec2 fragTexCoord;
out vec4 color;

uniform sampler2D screenTexture;
uniform float time;

void main()
{
    float scanline = sin(fragTexCoord.y * 800.0 + time * 2.0) * 0.02;
    vec4 texColor = texture(screenTexture, fragTexCoord);
    color = vec4(texColor.rgb - scanline, texColor.a);
}