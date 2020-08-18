uniform sampler2D source;
varying highp vec2 qt_TexCoord0;

uniform highp float mask;

void main(void)
{
    gl_FragColor = texture2D(source, qt_TexCoord0) * (abs(qt_TexCoord0.x - 0.5) < mask / 2.0 && qt_TexCoord0.y > 0.5 ? 0.0 : 1.0);
}
