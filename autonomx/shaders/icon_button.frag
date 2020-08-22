uniform mediump sampler2D source;
varying highp vec2 qt_TexCoord0;

uniform bool checked;

void main(void)
{
    vec4 c = texture2D(source, qt_TexCoord0);
    gl_FragColor = vec4(checked ? (vec3(1.0) - c.rgb)*c.a : c.rgb, c.a);
}
