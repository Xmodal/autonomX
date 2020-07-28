uniform mediump sampler2D source;
varying highp vec2 qt_TexCoord0;

uniform vec4 bg;

void main(void)
{
    vec2 st = qt_TexCoord0.st;
    gl_FragColor = mix(texture2D(source, st), bg, (st.x * 0.2) + 0.05);
}
