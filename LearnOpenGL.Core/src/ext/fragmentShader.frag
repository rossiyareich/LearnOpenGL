#version 440 core
out vec4 FragColor;

uniform vec3 objectColor = vec3(0.0);
uniform vec3 lightColor = vec3(1.0);

uniform float ambient;

in vec3 ourNormal;
in vec3 ourFragmentPosition;
uniform vec3 lightPosition;

uniform vec3 cameraPosition;
uniform float specular;
uniform uint shininess = 32;

void main()
{
    vec3 _ambient = ambient * lightColor;

    vec3 _normal = normalize(ourNormal);
    vec3 _lightDirection = normalize(lightPosition - ourFragmentPosition);
    float _diffuseCoeff = max(dot(_normal, _lightDirection), 0.0);
    vec3 _diffuse = _diffuseCoeff * lightColor;

    vec3 _viewDirection = normalize(cameraPosition - ourFragmentPosition);
    vec3 _reflectDirection = reflect(-_lightDirection, _normal);
    float _specularCoeff = pow(max(dot(_viewDirection, _reflectDirection), 0.0), shininess);
    vec3 _specular = specular * _specularCoeff * lightColor;

    vec3 _result = (_ambient + _diffuse + specular) * objectColor;
    FragColor = vec4(_result, 1.0);
} 
