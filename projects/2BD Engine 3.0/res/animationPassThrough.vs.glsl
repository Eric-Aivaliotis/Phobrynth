#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;

//create a position,color and normal variable for the other mesh
layout (location = 3) in vec3 inPosition2;
layout (location = 4) in vec4 inColor2;
layout (location = 5) in vec3 inNormal2;

layout (location = 6) out vec4 outColor;
layout (location = 7) out vec3 outNormal;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_ModelView;
uniform float time; //animation->SetUniform("time", animation->getTime());

void main() {
	outColor = inColor;
	outNormal = inNormal; //(a_ModelView * vec4(inNormal, 1)).xyz;
	outColor = inColor;
	gl_Position = a_ModelViewProjection * vec4(mix(inPosition,inPosition2,time), 1); 
}
