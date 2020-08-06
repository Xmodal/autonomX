attribute highp vec4 vertices;
varying highp vec2 coordinates;

void main() {
    gl_Position = vertices;
    coordinates = vertices.xy;
}
