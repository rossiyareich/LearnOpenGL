#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;

out vec4 ourColor;
out vec2 ourTexCoords;

uniform mat4 model, view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    ourTexCoords = aTexCoords;
}
