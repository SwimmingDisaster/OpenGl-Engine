#version 430 core

layout (location = 0) out vec4 gFragColor;

in flat int ObjectIndexFragment;

struct PointLight {
	vec3 color;
	float ambientIntensity;
	vec3 position;
	float diffuseIntensity;
	float constant;
	float linear;
	float exp;
	float dummy;
};
layout (std430, binding = 3) buffer lights{
	PointLight[] pointLightList;
};

uniform vec3 eyePos;
uniform vec2 gScreenSize;
uniform float specularPower;
uniform sampler2D gColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;


vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir, float shininess, vec3 color){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.exp * (distance * distance));
//	if(attenuation < 0.01f){
//		attenuation = 0.0f;
//	}
    // combine results
    vec3 ambient = light.ambientIntensity * color * light.color;
    vec3 diffuse = light.diffuseIntensity * diff * color * light.color;
	//float specValue = shininess * spec; 
	float specValue = 0.0f;
    vec3 specular = vec3(specValue, specValue, specValue);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec2 CalcTexCoord(){
    return gl_FragCoord.xy / gScreenSize;
}

void main() {
	vec2 texCoord = CalcTexCoord();
	vec3 worldPos = texture(gPosition, texCoord).xyz;
	float spec = texture(gPosition, texCoord).w;
	vec4 color = texture(gColor, texCoord);
	vec3 normal = texture(gNormal, texCoord).xyz;
	normal = normalize(normal);

    vec3 viewDir = normalize(eyePos - worldPos);

	vec3 finalColor = CalcPointLight(pointLightList[ObjectIndexFragment], worldPos, normal, viewDir, spec, color.xyz);
	/*
	vec3 finalColor = vec3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < pointLightList.length(); i++){
		finalColor += CalcPointLight(pointLightList[i], worldPos, normal, viewDir, spec, color.xyz);
	}
	*/
	//finalColor += CalcPointLight(pointLightList[ObjectIndexFragment], worldPos, normal, viewDir, spec, color.xyz);
	gFragColor = vec4(finalColor, 1.0f);

	//vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir, float shininess, vec3 color){
	// gFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//gColor = vec4(color[ObjectIndexFragment], 1.0f);
}

