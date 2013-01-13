#version 120

uniform mat4 gModelMatrix;
uniform mat4 gModelViewMatrix;
uniform mat4 gProjMatrix;

varying vec3 fragPosition;
varying vec2 fragTexCoord;
varying vec3 fragNormal;

void main(void)
{
	gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;
	fragPosition = (gModelMatrix * gl_Vertex).xyz;
	fragTexCoord = gl_MultiTexCoord0.xy;
	fragNormal = gl_Normal;
}