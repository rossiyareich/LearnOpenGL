#version 440 core
out vec4 FragColor;

in vec3 ourNormal;
in vec3 ourFragmentPosition;
uniform vec3 cameraPosition;

struct Material {
    vec3 objectColor;
    float ambient;
    float diffuse;
    float specular;
    uint shininess;
}; 
uniform Material material;

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
    float ambient;
    float diffuse;
    float specular;
};
uniform Light light;

void main()
{
    vec3 _ambient = light.ambient * material.ambient * light.lightColor;

    vec3 _normal = normalize(ourNormal);
    vec3 _lightDirection = normalize(light.lightPosition - ourFragmentPosition);
    float _diffuseCoeff = max(dot(_normal, _lightDirection), 0.0);
    vec3 _diffuse = light.diffuse * material.diffuse * (_diffuseCoeff * light.lightColor);

    vec3 _viewDirection = normalize(cameraPosition - ourFragmentPosition);
    vec3 _reflectDirection = reflect(-_lightDirection, _normal);
    float _specularCoeff = pow(max(dot(_viewDirection, _reflectDirection), 0.0), material.shininess);
    vec3 _specular = light.specular * material.specular * (_specularCoeff * light.lightColor);

    vec3 _result = (_ambient + _diffuse + _specular) * material.objectColor;
    FragColor = vec4(_result, 1.0);
}
