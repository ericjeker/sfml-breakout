uniform sampler2D texture;
uniform vec2 resolution;
uniform float glowStrength;

void main() {
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture2D(texture, texCoord);

    // Simple bloom effect
    vec4 glow = vec4(0.0);
    float offset = 1.0 / resolution.x;

    for(float x = -4.0; x <= 4.0; x++) {
        for(float y = -4.0; y <= 4.0; y++) {
            vec2 samplePos = texCoord + vec2(x, y) * offset;
            glow += texture2D(texture, samplePos) * glowStrength;
        }
    }

    gl_FragColor = color + glow / 81.0; // 9x9 kernel
}
