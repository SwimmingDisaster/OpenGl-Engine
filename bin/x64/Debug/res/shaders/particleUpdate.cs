#version 430 core


struct Test {
	vec4 pos_size;
	vec4 color;
	vec4 speed;
};


uniform float maxLifetime;
uniform float minLifetime;

uniform vec3 vRot;
uniform vec3 vPos;





layout(std430, binding = 2) buffer ParticleSSBO
{
	Test[100000] sizeList;
};

layout(std430, binding = 10) buffer particleIndexListSSBO
{
	int[10000] particleIndexList;
};

//uniform float deltaTime;



const float PI  = 3.14159265f;
int lastUsedParticle = 0;



float atan2(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(PI/2.0 - atan(x,y), atan(y,x), s);
}

float rand(float n){return fract(sin(n) * 43758.5453123);}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	int particleIndex = particleIndexList[ gl_GlobalInvocationID.x];

	/* glm:: */vec3 particleDir = /* glm::*/vec3(
	                            (1100.0f/*rand() % 2000*/ - 1000.0f) / 1000.0f,
	                            (1100.0f/*rand() % 2000*/ - 1000.0f) / 1000.0f,
	                            (1100.0f/*rand() % 2000*/ - 1000.0f) / 1000.0f
	                        );


	float x = 2 *0 - 1.0f;
	float y = 2 * 0 - 1.0f;
	float r = tan(60.0f/* angle */ * PI / 180.0f);

	particleDir = /* glm:: */vec3(r * x, r * y, 1);
	particleDir = /* glm:: */normalize(particleDir);
	//particleDir = /* glm:: */rotate(particleDir, glm::radians(vRot.y),/*  glm:: */vec3(0.0f, 1.0f, 0.0f));
	//particleDir = /* glm:: */rotate(particleDir,/*  glm:: */radians(vRot.x), /* glm:: */vec3(1.0f, 0.0f, 0.0f));
	//particleDir = /* glm:: */rotate(particleDir,/*  glm:: */radians(vRot.z), /* glm:: */vec3(0.0f, 0.0f, 1.0f));


	vec4 q;
	float angle = atan2( vRot.x, vRot.z );
	q.x= 0;
	q.y = 1 * sin( angle/2 );
	q.z = 0;
	q.w = cos( angle/2 );

	vec3 temp = cross(q.xyz, particleDir) + q.w * particleDir;
	particleDir = particleDir + 2.0*cross(q.xyz, temp);


	/* glm:: */vec3 particleSpeed = particleDir */*  glm:: */vec3(0.5f, 0.5f, 0.5f) * 2.0f;


	sizeList[particleIndex].pos_size.x = rand(particleIndex * 0.488f) * 100.0f;
	sizeList[particleIndex].pos_size.y = rand(particleIndex * 1.526f) * 100.0f;
	sizeList[particleIndex].pos_size.z = rand(particleIndex * 0.3805f) * 100.0f;

	sizeList[particleIndex].pos_size.w = rand(particleIndex * 0.8147f) * 3.0f + 1.1f;



	sizeList[particleIndex].color.x = 0.0f;
	sizeList[particleIndex].color.y = 0.0f;
	sizeList[particleIndex].color.z = 0.0f;
	sizeList[particleIndex].color.w = 0.0f;


	sizeList[particleIndex].speed.x = 0.0f;//rand(particleIndex * 0.123745f); // particleSpeed.x;
	sizeList[particleIndex].speed.y = 0.0f;//rand(particleIndex * 0.48049f); //particleSpeed.y;
	sizeList[particleIndex].speed.z =0.0f;// rand(particleIndex * 0.5893f); //particleSpeed.z;
	sizeList[particleIndex].speed.w =/*  (float) */0.75f/* (rand() % 5000) / 5000 */  * (maxLifetime - minLifetime) + minLifetime;
}