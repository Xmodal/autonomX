varying highp vec2 coords;

uniform sampler2D texture;

void main() {
    float intensity = texture2D(texture, coords * 0.25 + vec2(0.5, 0.5)).r;
    gl_FragColor = vec4(vec3(intensity), 1.0);
}
