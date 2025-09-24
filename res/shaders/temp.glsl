#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 mouse;
uniform float time;

out vec4 finalColor;

void main() {
    vec4 tex0Color = texture(texture0, fragTexCoord);

    vec2 uv = fragTexCoord * 2.0 - 1.0;

    float dist = length(uv);

    vec4 endColor = vec4(0, 0, 0, 1);

    float radius = (int(time * 200) % 200) / 100.;
    float thickness = 0.08;

    float ring = smoothstep(radius - thickness, radius, dist) -
            smoothstep(radius, radius + thickness, dist);

    vec4 endColor = vec4(ring, 0, 0, 1);
    endColor = mix(endColor, tex0Color, tex0Color.a);

    finalColor = endColor;
}
