#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 outlineColor;
uniform float thickness;
uniform float time;

out vec4 finalColor;

void main() {
    vec4 tex0Color = texture(texture0, fragTexCoord);
    vec2 texelSize = 1.0 / vec2(textureSize(texture0, 0));

    vec4 endColor = vec4(0, 0, 0, 0);

    vec2 offsets[8] = vec2[8](
            vec2(1, 1),
            vec2(0, 1),
            vec2(-1, 1),
            vec2(1, 0),
            vec2(-1, 0),
            vec2(1, -1),
            vec2(0, -1),
            vec2(-1, -1)
        );

    for (int i = 0; i < 8; i++) {
        vec2 offset = offsets[i] * thickness * texelSize;
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
    finalColor = endColor;
}

