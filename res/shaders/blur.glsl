#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 mouse;
uniform float time;

out vec4 finalColor;

void main() {
    vec4 tex0Color = texture(texture0, fragTexCoord);

    vec2 texelSize = 1.0 / vec2(textureSize(texture0, 0));

    float dist = distance(mouse, gl_FragCoord.xy);
    float radius = 100.0;
    float effect = smoothstep(radius, 0.0, dist);

    vec3 color = vec3(0, 0, 0);
    int size = int(effect * 100);

    finalColor = vec4(color, 1);
}
