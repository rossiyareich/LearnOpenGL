#version 440 core

#define CONSTANT 1.0
#define LINEAR 0.09
#define QUADRATIC 0.032

#define NR_GLOBALDIR_LIGHTS 2
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 2

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

struct LightData
{
    vec3 lightPosition;
    vec3 lightDirection;
    float lightInnerCutoff;
    float lightOuterCutoff;
};

struct Light {
    // 0 = global directional
    // 1 = point
    // 2 = spotlight
    uint lightType;
    LightData lightData;
    vec3 lightColor;
    float ambient;
    float diffuse;
    float specular;
};
uniform Light light;

uniform Light globalDirLights[NR_GLOBALDIR_LIGHTS];
uniform Light pointLights[NR_POINT_LIGHTS];
uniform Light spotLights[NR_SPOT_LIGHTS];

vec3 GetPhong(Light light, vec3 _lightDirection, float _attenuation, float _nonAmbientCoeff)
{
    vec3 _ambient = light.ambient * material.ambient * light.lightColor;

    vec3 _normal = normalize(ourNormal);
    float _diffuseCoeff = max(dot(_normal, _lightDirection), 0.0);
    vec3 _diffuse = light.diffuse * material.diffuse * (_diffuseCoeff * light.lightColor);

    vec3 _viewDirection = normalize(cameraPosition - ourFragmentPosition);
    vec3 _reflectDirection = reflect(-_lightDirection, _normal);
    float _specularCoeff = pow(max(dot(_viewDirection, _reflectDirection), 0.0), material.shininess);
    vec3 _specular = light.specular * material.specular * (_specularCoeff * light.lightColor);

    _ambient *= _attenuation;
    _diffuse *= _attenuation * _nonAmbientCoeff;
    _specular *= _attenuation * _nonAmbientCoeff;

    return (_ambient + _diffuse + _specular);
}

vec3 CalcGlobalDirLight(Light light)
{
    vec3 _lightDirection;
    float _attenuation;
    float _nonAmbientCoeff = 1.0;

    _lightDirection = normalize(-light.lightData.lightDirection);
    _attenuation = 1.0;

    return GetPhong(light, _lightDirection, _attenuation, _nonAmbientCoeff);
}

vec3 CalcPointLight(Light light)
{
    vec3 _lightDirection;
    float _attenuation;
    float _nonAmbientCoeff = 1.0;

    _lightDirection = normalize(light.lightData.lightPosition - ourFragmentPosition);

    float _distance = length(light.lightData.lightPosition - ourFragmentPosition);
    _attenuation = 1.0 / (CONSTANT + LINEAR * _distance + QUADRATIC * (_distance * _distance));

    return GetPhong(light, _lightDirection, _attenuation, _nonAmbientCoeff);
}

vec3 CalcSpotlight(Light light)
{
    vec3 _lightDirection;
    float _attenuation;
    float _nonAmbientCoeff = 1.0;

    _lightDirection = normalize(light.lightData.lightPosition - ourFragmentPosition);

    float _distance = length(light.lightData.lightPosition - ourFragmentPosition);
    _attenuation = 1.0 / (CONSTANT + LINEAR * _distance + QUADRATIC * (_distance * _distance));

    float _theta = dot(_lightDirection, normalize(-light.lightData.lightDirection));
    float _epsilon = light.lightData.lightInnerCutoff - light.lightData.lightOuterCutoff;
    _nonAmbientCoeff = clamp((_theta - light.lightData.lightOuterCutoff) / _epsilon, 0.0, 1.0);

    return GetPhong(light, _lightDirection, _attenuation, _nonAmbientCoeff);
}

void main()
{
    vec3 result;

    for(int i = 0; i < NR_GLOBALDIR_LIGHTS; i++)
        result += CalcGlobalDirLight(globalDirLights[i]);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i]);

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotlight(spotLights[i]);

    FragColor = vec4(result, 1.0);
}
