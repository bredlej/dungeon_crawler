#version 330 core

in vec2 fragTexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec4 outline_color = vec4(0.0, 1.0, 0.0, 1.0); // Time value to animate the glow effect

void main()
{
    vec4 texColor = texture(texture1, fragTexCoord);

    if (texColor.a == 1.0) {
       FragColor = outline_color;
    } else {
        FragColor = texColor;
    }
}