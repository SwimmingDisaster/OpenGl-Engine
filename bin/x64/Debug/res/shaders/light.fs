#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct DirLight {
    vec4 direction_junk;
    
    vec4 ambient_junk;
    vec4 diffuse_junk;
    vec4 specular_junk;
    vec4 intensities;
};


struct PointLight {
    vec4 position_junk;
    vec4 ambient_constant;
    vec4 diffuse_linear;
    vec4 specular_quadratic;
    vec4 intensities;
};

struct SpotLight {
    vec4 position_cutOff;
    vec4 direction_outerCutOff;
  
    vec4 ambient_constant;
    vec4 diffuse_linear;
    vec4 specular_quadratic; 
    vec4 intensities;      
};

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 10
#define NR_SPOT_LIGHTS 1


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform float dirLightNumber;
uniform float pointLightNumber;
uniform float spotLightNumber;

//uniform DirLight dirLight[NR_DIR_LIGHTS];
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight[NR_SPOT_LIGHTS];

uniform Material material;



layout (std140, binding = 3) uniform DirLights
{
    DirLight[NR_DIR_LIGHTS] dirLights;
};

layout (std140, binding = 4) uniform PointLights
{
    PointLight[NR_POINT_LIGHTS] pointLights;
};

layout (std140, binding = 5) uniform SpotLights
{
    SpotLight[NR_SPOT_LIGHTS] spotLights;
};



vec3 norm;
vec3 viewDir;

// function prototypes
vec3 CalcDirLight(DirLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);

void main()
{    
    // properties
    norm = normalize(Normal);
    viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for(int i = 0; i < dirLightNumber; i++)
        result += CalcDirLight(dirLights[i]);   
   

    for(int i = 0; i < pointLightNumber; i++)
        result += CalcPointLight(pointLights[i]);    

   for(int i = 0; i < spotLightNumber; i++)
        result += CalcSpotLight(spotLights[i]);   
    FragColor = vec4(result , 1.0);
}

vec3 CalcDirLight(DirLight light)
{
    vec3 lightDir = normalize(-light.direction_junk.xyz);
    // diffuse shading
    float diff = max(dot(norm.xyz, lightDir.xyz), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir.xyz, norm.xyz);
    float spec = pow(max(dot(viewDir.xyz, reflectDir.xyz), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient_junk.xyz * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse_junk.xyz * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular_junk.xyz * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient * light.intensities.x + diffuse* light.intensities.y + specular* light.intensities.z);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position_junk.xyz - FragPos);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position_junk.xyz - FragPos);
    float attenuation = 1.0 / (light.ambient_constant.w + light.diffuse_linear.w * distance + light.specular_quadratic.w * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient_constant.xyz * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse_linear.xyz * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular_quadratic.xyz * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;
    return (ambient* light.intensities.x +diffuse* light.intensities.y+ specular* light.intensities.z);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light)
{
    vec3 lightDir = normalize(light.position_cutOff.xyz - FragPos);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position_cutOff.xyz - FragPos);
    float attenuation = 1.0 / (light.ambient_constant.w + light.diffuse_linear.w * distance + light.specular_quadratic.w * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction_outerCutOff.xyz)); 
    float epsilon = light.position_cutOff.w - light.direction_outerCutOff.w;
    float intensity = clamp((theta - light.direction_outerCutOff.w) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient_constant.xyz * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse_linear.xyz * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular_quadratic.xyz * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient * light.intensities.x+ diffuse* light.intensities.y + specular* light.intensities.z);
}