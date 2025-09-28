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

    vec4 outlineColor = vec4(
            0.7 + 0.3 * sin(time * 0.5),
            0.0, // 0.5 + 0.5 * cos(time * 1.3),
            0.2 + 0.8 * sin(time * 0.9),
            1.0
        );

    float value = sin(time * 2) / 2 + 0.5;
    int thickness = 16 + int(value * 8) * 2;

    vec4 endColor = vec4(0, 0, 0, 0);

    for (int y = -thickness; y <= thickness; y++) {
        for (int x = -thickness; x <= thickness; x++) {
            if (abs(x) != thickness && abs(y) != thickness) {
                continue; // skip interior
            }

            vec2 offset = vec2(x, y) * texelSize;
            vec2 position = fragTexCoord.xy + offset;

            if (position.x < 0.0 || position.x > 1.0 ||
                    position.y < 0.0 || position.y > 1.0) {
                continue;
            }

            if (texture(texture0, position).a != 0.0) {
                endColor = outlineColor;
            }
        }
    }

    if (tex0Color.a == 0.0 && endColor.a == 0.0) discard;

    endColor = mix(endColor, tex0Color, tex0Color.a);
    finalColor = endColor;
}
