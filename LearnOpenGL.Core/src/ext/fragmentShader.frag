#version 440 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 ourTexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, ourTexCoords), texture(texture2, ourTexCoords), 0.2);
} 
