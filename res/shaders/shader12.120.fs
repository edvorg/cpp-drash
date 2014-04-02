#version 120

// mrt shader for deferred lighting

uniform sampler2D gTex1;
uniform sampler2D gTex2;

uniform vec3 gLight1Position;
uniform vec3 gLight1Direction;
uniform float gLight1Angle;

varying vec2 fragTexCoord;
varying vec3 fragPosition;

varying vec3 aViewDirTangent;
varying vec3 aLight1PosTangent;
varying vec3 aLight1Direction;

void main(void)
{
	gl_FragData[0] = texture2D(gTex1, fragTexCoord);
    gl_FragData[1] = texture2D(gTex2, fragTexCoord);
    gl_FragData[2] = vec4(1, 0, 0, 1);
}
