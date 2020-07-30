varying highp vec2 coords;

void main() {
    float inside = coords.x + coords.y < 0.0 ? 1.0 : 0.0;
    gl_FragColor = vec4(coords.x, coords.y, inside, 1.0);
}
