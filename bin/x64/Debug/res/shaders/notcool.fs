#version 430 core

out vec4 FragColor;


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 resolution;
uniform float treshold;

uniform vec3 baseColor;
uniform vec3 transformColor;
uniform vec3 edgeColor;
uniform float width;


float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

float surface3 ( vec3 coord ) {
        float frequency = 4.0;
        float n = 0.0;  

        n += 1.0    * abs( noise( coord.xy * frequency) );
        n += 0.5    * abs( noise( coord.xy * frequency * 2.0 ) );
        n += 0.25   * abs( noise( coord.xy * frequency * 4.0 ) );

        return n;
}

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}


const float F3 =  0.3333333;
const float G3 =  0.1666667;

float simplex3d(vec3 p) {
	 
	 vec3 s = floor(p + dot(p, vec3(F3)));
	 vec3 x = p - s + dot(s, vec3(G3));
	 
	 vec3 e = step(vec3(0.0), x - x.yzx);
	 vec3 i1 = e*(1.0 - e.zxy);
	 vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	 vec3 x1 = x - i1 + G3;
	 vec3 x2 = x - i2 + 2.0*G3;
	 vec3 x3 = x - 1.0 + 3.0*G3;
	 
	 vec4 w, d;
	 
	 w.x = dot(x, x);
	 w.y = dot(x1, x1);
	 w.z = dot(x2, x2);
	 w.w = dot(x3, x3);
	 
	 w = max(0.6 - w, 0.0);
	 
	 d.x = dot(random3(s), x);
	 d.y = dot(random3(s + i1), x1);
	 d.z = dot(random3(s + i2), x2);
	 d.w = dot(random3(s + 1.0), x3);
	 
	 w *= w;
	 w *= w;
	 d *= w;
	 
	 return dot(d, vec4(52.0));
}



void main() {


    float n =  simplex3d(FragPos.xyz / resolution.xyz);
   	n = (n +1) /2;


  	if(n < treshold){
		FragColor = vec4(baseColor, 1.0);
	}
	else{
		FragColor = vec4(edgeColor, 1.0);
	} 
	n -= width;
	if(treshold <= 0.003){
		discard;
	}
	if(n > treshold){
		discard;
	}  

/* 	if(FragPos.y < treshold*15.5){
		FragColor = vec4(baseColor, 1.0);
	}
	else{
		FragColor = vec4(transformColor, 1.0);
	} */

}
