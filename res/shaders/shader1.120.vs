#version 120

uniform mat4 gModelViewMatrix;
uniform mat4 gProjMatrix;

varying vec3 fragNormal;

void main(void)
{
	gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;
	fragNormal = (gModelViewMatrix * vec4(gl_Normal, 0)).xyz;
	fragNormal = normalize(fragNormal);
}