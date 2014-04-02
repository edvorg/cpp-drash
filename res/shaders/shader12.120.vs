#version 120

// mrt shader for deferred lighting

uniform mat4 gModelMatrix;
uniform mat4 gModelViewMatrix;
uniform mat4 gProjMatrix;

uniform vec3 gViewPosition;
uniform vec3 gLight1Position;
uniform vec3 gLight1Direction;
uniform float gLight1Angle;

attribute vec3 gTangent;
attribute vec3 gBinormal;

varying vec2 fragTexCoord;
varying vec3 fragPosition;

varying vec3 aViewDirTangent;
varying vec3 aLight1PosTangent;
varying vec3 aLight1Direction;

void main(void)
{
	gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;

	fragTexCoord = gl_MultiTexCoord0.xy;
	fragPosition = (gModelMatrix * gl_Vertex).xyz;

	vec3 normal = normalize((gModelMatrix * vec4(gl_Normal, 0)).xyz);
	vec3 binormal = normalize((gModelMatrix * vec4(gBinormal, 0)).xyz);
	vec3 tangent = normalize((gModelMatrix * vec4(gTangent, 0)).xyz);

	vec3 viewDir = fragPosition - gViewPosition;
	vec3 light1Dir = gLight1Position - fragPosition;
			  
	aViewDirTangent =
        vec3(tangent.x * viewDir.x + tangent.y * viewDir.y + tangent.z * viewDir.z,
             binormal.x * viewDir.x + binormal.y * viewDir.y + binormal.z * viewDir.z, 
             normal.x * viewDir.x + normal.y * viewDir.y + normal.z * viewDir.z);

	aLight1PosTangent =
        vec3(tangent.x * light1Dir.x + tangent.y * light1Dir.y + tangent.z * light1Dir.z,
             binormal.x * light1Dir.x + binormal.y * light1Dir.y + binormal.z * light1Dir.z, 
             normal.x * light1Dir.x + normal.y * light1Dir.y + normal.z * light1Dir.z);

	aLight1Direction =
        vec3(tangent.x * gLight1Direction.x + tangent.y * gLight1Direction.y + tangent.z * gLight1Direction.z,
             binormal.x * gLight1Direction.x + binormal.y * gLight1Direction.y + binormal.z * gLight1Direction.z, 
             normal.x * gLight1Direction.x + normal.y * gLight1Direction.y + normal.z * gLight1Direction.z);
}
