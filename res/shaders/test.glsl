#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec2 mouse;
uniform float time;

out vec4 finalColor;

float random2d(vec2 coord) {
    return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec4 tex0Color = texture(texture0, fragTexCoord);
    vec4 tex1Color = vec4(1 - tex0Color.rgb, tex0Color.a); 

    vec2 texelSize = 1.0 / vec2(fragTexCoord * time);

    float value = clamp(random2d(fragTexCoord * time) / 10, 0, 0.2);

    vec2 light_position = vec2(cos(time + value), sin(time * 2)) * 0.5 + 0.5;
    float light_strength = distance(light_position * 5, fragTexCoord.xy * 5) * 1;
    light_strength *= light_strength;
    light_strength = clamp(light_strength, 0, 1);
    light_strength = 1 - light_strength;

    vec4 color = vec4(value, value, value, 1);
    color = mix(color, tex0Color, light_strength);

    float dist = distance(mouse, gl_FragCoord.xy);
    float radius = 100.0;
    float effect = smoothstep(radius, 0.0, dist);

    light_strength = clamp(light_strength, 0, 0.5) * 2;
    light_strength = 1 - light_strength;

    float strength = effect * light_strength;

    vec4 blacklight = tex0Color * 0.1 + tex1Color * 0.8;

    finalColor = mix(color, blacklight, strength);
}
