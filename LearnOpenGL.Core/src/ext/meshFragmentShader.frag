#version 440 core

out vec4 FragColor;
in vec2 ourTexCoords;

uniform sampler2D texture_diffuse0;

void main()
{
    FragColor = texture(texture_diffuse0, ourTexCoords);
}
