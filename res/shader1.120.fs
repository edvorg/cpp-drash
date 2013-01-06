#version 120

varying vec3 fragNormal;

void main(void)
{
	gl_FragColor = vec4(0.5 + (fragNormal * 0.5), 1);
}