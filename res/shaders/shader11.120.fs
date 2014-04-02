#version 120

// deferred shading 

uniform sampler2D gTex1;
uniform sampler2D gTex2;
uniform sampler2D gTex3;

uniform vec3 gLight1Position;
uniform vec3 gLight1Direction;
uniform float gLight1Angle;

varying vec2 fragTexCoord;

const float gAmbientKoeff = 0.05;
const float gDiffuseKoeff = 1.0;
const float gAttenuation = 0.01;
const vec3 gLightColor = vec3(1, 1, 1);
const float gSpecularPower = 2;
const float gSpecularKoeff = 0.3;

void main(void)
{
    vec4 diffuseColor = texture2D(gTex1, fragTexCoord);
	vec3 normal = texture2D(gTex2, fragTexCoord).xyz;
	vec3 lightDir = texture2D(gTex3, fragTexCoord).xyz;
	vec3 reflectDir = reflect(lightDir, normal);
	// vec3 viewDir = normalize(aViewDirTangentSpace);

	float diffuseKoeff = clamp(dot(lightDir, normal), 0, 1) // /
	                     // pow(length(gLight1Position - fragPosition), gAttenuation)
        * gDiffuseKoeff;

	float specularKoeff = 0;//pow(clamp(dot(reflectDir, viewDir), 0, 1), gSpecularPower) * gSpecularKoeff;

	gl_FragColor = vec4(diffuseColor.xyz * (gAmbientKoeff + diffuseKoeff) +
			    gLightColor * specularKoeff, diffuseColor.w);
}
