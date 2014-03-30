#version 120

// simple textured object

uniform sampler2D gTex1;
varying vec2 fragTexCoord;

void main(void)
{
	gl_FragColor = texture2D(gTex1, fragTexCoord);
}
