#version 440 core
out vec4 FragColor;

in vec3 ourNormal;
in vec3 ourFragmentPosition;
in vec2 ourTexCoords;
uniform vec3 cameraPosition;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D emission;
uniform float time;
struct TexturedMaterial
{
    uint shininess;
}; 
uniform TexturedMaterial texturedMaterial;

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
    // Ambient
    vec3 _ambient = light.ambient * texture(diffuse, ourTexCoords).rgb * light.lightColor;

    // Diffuse
    vec3 _normal = normalize(ourNormal);
    vec3 _lightDirection = normalize(light.lightPosition - ourFragmentPosition);
    float _diffuseCoeff = max(dot(_normal, _lightDirection), 0.0);
    vec3 _diffuse = light.diffuse * texture(diffuse, ourTexCoords).rgb  * (_diffuseCoeff * light.lightColor);

    // Specular
    vec3 _viewDirection = normalize(cameraPosition - ourFragmentPosition);
    vec3 _reflectDirection = reflect(-_lightDirection, _normal);
    float _specularCoeff = pow(max(dot(_viewDirection, _reflectDirection), 0.0), texturedMaterial.shininess);
    vec3 _specular = light.specular * texture(specular, ourTexCoords).rgb  * (_specularCoeff * light.lightColor) ;

    // Emission
    vec3 _emission = vec3(0.0);
    if(texture(specular, ourTexCoords).rgb == vec3(0.0))
    {
        _emission = texture(emission, ourTexCoords + vec2(0.0, time)).rgb;
        _emission *= (sin(time) * 0.5 + 0.5) * 2.0;
    }

    FragColor = vec4(_ambient + _diffuse + _specular + _emission, 1.0);
}
