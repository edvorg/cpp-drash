#version 120

uniform sampler2D gTex1;

varying vec2 texCoords;

void main(void)
{
	gl_FragColor = texture2D(gTex1, texCoords);
}