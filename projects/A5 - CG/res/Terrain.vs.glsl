#version 410
//Basically HeightMap.vs.glsl only I just kept the tutorial variable names, just to manage easier
layout (location = 0) in vec3 inPosition; //vertex position
layout (location = 1) in vec4 inColor; //vertex color
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUV; //vertex uv

layout (location = 0) out vec4 outColor; //color
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outWorldPos;
layout (location = 3) out vec2 outUV; //texture UV
layout (location = 4) out vec3 outTexWeights;

uniform mat4 a_ModelViewProjection; //MVP
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;

//Height Map
uniform sampler2D myTextureSampler;
uniform float height;

void main() {
	//Height Map
	float outHeight = texture(myTextureSampler, inUV).r;
	
	//v.z = (texture(myTextureSampler, inUV).r);
	vec3 v = vec3(inPosition.x, inPosition.y, outHeight * height);

	outColor = inColor;
	outNormal = a_NormalMatrix * inNormal;
	outColor = inColor;
	outWorldPos = v;
	gl_Position = a_ModelViewProjection * vec4(v, 1);
	
	outTexWeights = vec3(
	    clamp((-outHeight + 0.5f) * 4.0f, 0.0f, 1.0f),
	    min(clamp((-outHeight + 0.75f) * 4.0f, 0.0f, 1.0f), clamp((outHeight - 0.25f) * 4.0f, 0.0f, 1.0f)),
	    clamp((outHeight - 0.5f) * 4.0f, 0.0f, 1.0f)
	);

	//outUV = inUV;
	outUV = v.xy;
}