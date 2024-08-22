#version 330 core

uniform sampler2D image;
uniform float threshold;
in vec2 fragTexCoord;
out vec4 FragColor;

void main() {
    vec4 color = texture(image, fragTexCoord);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > threshold) {
        FragColor = color;
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}