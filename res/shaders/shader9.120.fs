#version 120

// radial blur

uniform sampler2D gTex1;
uniform vec3 gLight1Position;
varying vec2 fragTexCoord;

vec2 center = vec2(0.5, 0.5);
vec2 right = vec2(1, 0);

void main(void)
{
    vec2 fromCenter = fragTexCoord - center;
    float distanceFromCenter = length(fromCenter);
    float angle = acos(dot(normalize(fromCenter), right));
    if (fromCenter.y < 0) angle = -angle;

    vec4 sum = vec4(0, 0, 0, 0);
    int iters = 20;
    for (float i = -iters/2; i < iters/2; i++) {

        float currentAngle = angle + i * gLight1Position.x * 0.1;
        vec2 currentCoord =
            center + vec2(cos(currentAngle), sin(currentAngle)) *
            distanceFromCenter;
        sum += texture2D(gTex1, currentCoord);
    }

    sum /= iters;
    
	gl_FragColor = sum;
}
