#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float time;

out vec4 finalColor;

vec4 hue2rgb(float h) {
    float k = h * 6.0;
    int i = int(floor(k));
    float f = k - float(i);

    vec3 rgb;
    if (i == 0) rgb = vec3(1.0, f, 0.0);
    else if (i == 1) rgb = vec3(1.0 - f, 1.0, 0.0);
    else if (i == 2) rgb = vec3(0.0, 1.0, f);
    else if (i == 3) rgb = vec3(0.0, 1.0 - f, 1.0);
    else if (i == 4) rgb = vec3(f, 0.0, 1.0);
    else rgb = vec3(1.0, 0.0, 1.0 - f);

    return vec4(rgb, 1);
}

void main() {
    vec4 tex0Color = texture(texture0, fragTexCoord);
    vec2 texelSize = 1.0 / vec2(textureSize(texture0, 0));

    float value = sin(time * 2.0) * 0.5 + 0.5;
    float thickness = 8.0 + floor(value * 120.0) * 1.0;

    vec4 outlineColor = vec4(
            0.7 + 0.3 * sin(time * 0.5),
            0.0, // 0.5 + 0.5 * cos(time * 1.3),
            0.2 + 0.8 * sin(time * 0.9),
            1.0
        );

    vec4 endColor = vec4(0, 0, 0, 0);

    const int steps = 360;
    for (int i = 0; i < steps; i++) {
        float angle = 360 * (float(i) / float(steps));
        vec2 offset = vec2(
                cos(angle),
                sin(angle)
            ) * thickness * texelSize;

        vec2 position = fragTexCoord.xy + offset;

        if (position.x < 0 ||
                position.x > 1 ||
                position.y < 0 ||
                position.y > 1)
            continue;

        if (texture(texture0, position).a != 0) {
            endColor = outlineColor;
            break;
        }
    }

    if (tex0Color.a == 0.0 && endColor.a == 0.0) discard;

    endColor = mix(endColor, tex0Color, tex0Color.a);

    if (tex0Color.a == 1 &&
            tex0Color.r > tex0Color.g + 0.156 &&
            tex0Color.r > tex0Color.b + 0.156)
        endColor = outlineColor;

    finalColor = endColor;
}
