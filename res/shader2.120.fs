#version 120

uniform sampler2D gTex1;

uniform vec3 gLight1Position;

uniform mat4 gModelMatrix;

varying vec3 fragPosition;
varying vec2 fragTexCoord;
varying vec3 fragNormal;

const float gAmbientKoeff = 0.15;

void main(void)
{
	vec4 diffuseColor = texture2D(gTex1, fragTexCoord);
	vec3 transformedNormal = normalize((gModelMatrix * vec4(fragNormal, 0)).xyz);

	vec3 lightDir = normalize(gLight1Position - fragPosition);

	float diffuseKoeff = clamp(dot(lightDir, transformedNormal), 0, 1);

	gl_FragColor = vec4(diffuseColor.xyz * (diffuseKoeff + gAmbientKoeff), 1);
}