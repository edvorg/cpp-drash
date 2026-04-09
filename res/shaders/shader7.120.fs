#version 120

/// point light shader with normal mapping. gLightColor is uploaded
/// from PointLight::color or SpotLight::color (defaults to white) so
/// coloured emergency lights actually emit their colour, but otherwise
/// this shader keeps the original drash lighting behaviour: no
/// distance falloff, ambient tinted by the light colour.

uniform sampler2D gTex1;
uniform sampler2D gTex2;
uniform sampler2D gTex3;

uniform vec3 gLight1Position;
uniform vec3 gLightColor;

varying vec2 fragTexCoord;
varying vec3 fragPosition;

varying vec3 aViewDirTangentSpace;
varying vec3 aLight1Dir;

const float gAmbientKoeff = 0.2;
const float gDiffuseKoeff = 1.0;
const float gAttenuation = 0.01;
const float gSpecularPower = 32;

void main(void)
{
	vec4 diffuseColor = texture2D(gTex1, fragTexCoord);
	vec3 normal = normalize(texture2D(gTex2, fragTexCoord).xyz * 2.0 - 1.0);
	vec4 specularColor = texture2D(gTex3, fragTexCoord);
	vec3 lightDir = normalize(aLight1Dir);
	vec3 reflectDir = reflect(lightDir, normal);
	vec3 viewDir = normalize(aViewDirTangentSpace);

	float diffuseKoeff = clamp(dot(lightDir, normal), 0, 1) /
	                     pow(length(gLight1Position - fragPosition), gAttenuation) * gDiffuseKoeff;

	float specularKoeff = pow(clamp(dot(reflectDir, viewDir), 0, 1), gSpecularPower) *
	                      normalize(specularColor.xyz).x;

	gl_FragColor = vec4(diffuseColor.xyz * gLightColor * gAmbientKoeff +
	                    diffuseColor.xyz * gLightColor * diffuseKoeff +
	                    gLightColor * specularKoeff,
	                    diffuseColor.w);
}
