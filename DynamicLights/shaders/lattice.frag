varying highp vec2 coords;

void main() {
    gl_FragColor = vec4(coords.x, coords.y, 1.0, 1.0);
}
