#version 120

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

const float gAmbientKoeff = 0.05;
const float gDiffuseKoeff = 1.0;
const float gAttenuation = 0.01;
const vec3 gLightColor = vec3(1, 1, 1);
const float gSpecularPower = 2;
const float gSpecularKoeff = 0.3;

void main(void)
{
	vec4 diffuseColor = texture2D(gTex1, fragTexCoord);
	vec3 normal = normalize(texture2D(gTex2, fragTexCoord).xyz * 2.0 - 1.0);
	vec3 lightDir = normalize(aLight1PosTangent);
	vec3 reflectDir = reflect(lightDir, normal);
	vec3 viewDir = normalize(aViewDirTangent);

	float spotKoeff = pow(clamp(dot(normalize(-gLight1Direction), normalize(gLight1Position - fragPosition)), 0, 1), 2);

	float diffuseKoeff = clamp(dot(lightDir, normal), 0, 1) /
	                     pow(length(gLight1Position - fragPosition), gAttenuation) * gDiffuseKoeff;

	float specularKoeff = pow(clamp(dot(reflectDir, viewDir), 0, 1), gSpecularPower) * gSpecularKoeff;

	gl_FragColor = vec4((diffuseColor.xyz * (gAmbientKoeff + diffuseKoeff * spotKoeff) +
			    gLightColor * specularKoeff * spotKoeff), diffuseColor.w);
}
