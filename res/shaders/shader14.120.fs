#version 120

/// Dual-light shader for the "powerless" test scene. Light 1 is the
/// player's white "torch" (point light, strong distance falloff so it
/// only illuminates the area immediately around the player). Light 2
/// is an area-fill emergency light from the closest hanging lamp
/// (weak falloff, coloured). The two contributions are summed so the
/// player can carry a torch through a red-lit chamber. Other tests
/// use shader7 with the original single-light no-falloff behaviour.

uniform sampler2D gTex1;
uniform sampler2D gTex2;
uniform sampler2D gTex3;

uniform vec3 gLight1Position;
uniform vec3 gLight2Position;
uniform vec3 gLightColor;
uniform vec3 gLight2Color;

varying vec2 fragTexCoord;
varying vec3 fragPosition;

varying vec3 aViewDirTangentSpace;
varying vec3 aLight1Dir;
varying vec3 aLight2Dir;

const float gAmbientKoeff = 0.22;
const float gDiffuseKoeff = 1.0;
const float gFalloffK1 = 0.07;   // light 1 (torch): short range
const float gFalloffK2 = 0.004;  // light 2 (area):  long range
const float gSpecularPower = 32;

void main(void)
{
	vec4 diffuseColor = texture2D(gTex1, fragTexCoord);
	vec3 normal = normalize(texture2D(gTex2, fragTexCoord).xyz * 2.0 - 1.0);
	vec4 specularColor = texture2D(gTex3, fragTexCoord);

	vec3 viewDir = normalize(aViewDirTangentSpace);

	// --- light 1 (player torch) ---
	vec3 lightDir1 = normalize(aLight1Dir);
	vec3 reflectDir1 = reflect(lightDir1, normal);
	float dist1 = length(gLight1Position - fragPosition);
	float falloff1 = 1.0 / (1.0 + gFalloffK1 * dist1 * dist1);
	float diffuseKoeff1 = clamp(dot(lightDir1, normal), 0, 1) * falloff1 * gDiffuseKoeff;
	float specularKoeff1 = pow(clamp(dot(reflectDir1, viewDir), 0, 1), gSpecularPower)
	                     * normalize(specularColor.xyz).x * falloff1;

	// --- light 2 (atmosphere / lamp) ---
	vec3 lightDir2 = normalize(aLight2Dir);
	vec3 reflectDir2 = reflect(lightDir2, normal);
	float dist2 = length(gLight2Position - fragPosition);
	float falloff2 = 1.0 / (1.0 + gFalloffK2 * dist2 * dist2);
	float diffuseKoeff2 = clamp(dot(lightDir2, normal), 0, 1) * falloff2 * gDiffuseKoeff;
	float specularKoeff2 = pow(clamp(dot(reflectDir2, viewDir), 0, 1), gSpecularPower)
	                     * normalize(specularColor.xyz).x * falloff2;

	// Ambient is tinted by the SUM of both light colours so a chamber
	// with both a torch and a red emergency lamp reads as red-tinted
	// even where direct lighting can't reach the geometry.
	vec3 ambient = diffuseColor.xyz * (gLightColor + gLight2Color) * gAmbientKoeff;

	vec3 diffuse1 = diffuseColor.xyz * gLightColor  * diffuseKoeff1;
	vec3 diffuse2 = diffuseColor.xyz * gLight2Color * diffuseKoeff2;

	vec3 spec1 = gLightColor  * specularKoeff1;
	vec3 spec2 = gLight2Color * specularKoeff2;

	gl_FragColor = vec4(ambient + diffuse1 + diffuse2 + spec1 + spec2,
	                    diffuseColor.w);
}
