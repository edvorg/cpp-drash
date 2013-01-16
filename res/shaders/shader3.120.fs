#version 120

uniform sampler2D gTex1;
uniform sampler2D gTex2;

uniform vec3 gLight1Position;
uniform mat4 gModelMatrix;

varying vec2 fragTexCoord;
varying vec3 fragPosition;
varying vec3 aLight1Dir;

const float gAmbientKoeff = 0.1;
const float gAttenuation = 0.01;

void main(void)
{
	vec4 diffuseColor = texture2D(gTex1, fragTexCoord);

	vec3 nn = (texture2D(gTex2, fragTexCoord).xyz * 2.0) - 1.0;

	float diffuseKoeff = clamp(dot(normalize(aLight1Dir),
				       normalize(nn)),
                                   0,
                                   1);

	float lengthKoeff = 1.0 / pow(length(gLight1Position - fragPosition), gAttenuation);

	gl_FragColor = vec4(diffuseColor.xyz * (gAmbientKoeff + diffuseKoeff * lengthKoeff), diffuseColor.w);
}