#version 430 core

layout (location = 0) out vec4 gFragColor;

in vec3 FragPos;
in flat int ObjectIndexFragment;

struct PointLight {
	vec3 color;
	float ambientIntensity;
	vec3 position;
	float diffuseIntensity;
	float constant;
	float linear;
	float exp;
};
layout (std140, binding = 3) uniform lights{
	PointLight[32] pointLightList;
};

uniform vec3 eyePos;
uniform vec2 gScreenSize;
uniform float specularPower;
uniform sampler2D gColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;

vec4 CalcLightInternal(PointLight light, vec3 lightDirection, vec3 worldPos, vec3 normal, float specularIntensity){
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
//	float diffuseFactor = dot(normal, -lightDirection);
	float diffuseFactor = 1.0f;

	vec4 diffuseColor  = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0) {
		diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

		vec3 vertexToEye = normalize(eyePos - worldPos);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(vertexToEye, lightReflect);
		specularFactor = pow(specularFactor, specularPower);
		if (specularFactor > 0) {
			specularColor = vec4(light.color, 1.0f) * specularIntensity * specularFactor;
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}
vec4 CalcPointLight(PointLight pointLight, vec3 worldPos, vec3 normal, float specularIntensity){
	vec3 lightDirection = worldPos - pointLight.position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 color = CalcLightInternal(pointLight, lightDirection, worldPos, normal, specularIntensity);

	float attenuation =  pointLight.constant +
						 pointLight.linear * distance +
						 pointLight.exp * distance * distance;

	attenuation = max(1.0, attenuation);

	return color / attenuation;
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

	gFragColor = color * CalcPointLight(pointLightList[ObjectIndexFragment], worldPos, normal, spec);
	//gColor = vec4(color[ObjectIndexFragment], 1.0f);
}

