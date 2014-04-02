#version 120

// simple vertex shader for simple textured drawing

uniform mat4 gModelViewMatrix;
uniform mat4 gProjMatrix;
varying vec2 fragTexCoord;

void main(void)
{
	gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;
	fragTexCoord = gl_MultiTexCoord0.xy;
}
