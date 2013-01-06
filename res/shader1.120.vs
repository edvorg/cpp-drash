#version 120

uniform mat4 gModelViewMatrix;
uniform mat4 gProjMatrix;

varying vec2 texCoords;

void main(void)
{
	gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;
	texCoords = gl_MultiTexCoord0.xy;
}