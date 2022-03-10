#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model, view, projection;

out vec3 ourNormal;
out vec3 ourFragmentPosition;
out vec2 ourTexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourNormal = mat3(transpose(inverse(model))) * aNormal;
    ourFragmentPosition = vec3(model * vec4(aPos, 1.0)); // Get world space vertex position
    ourTexCoords = aTexCoords;
}
