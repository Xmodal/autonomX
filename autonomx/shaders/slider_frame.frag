uniform sampler2D source;
varying highp vec2 qt_TexCoord0;

uniform highp float maskWidth;

void main(void)
{
    gl_FragColor = texture2D(source, qt_TexCoord0) * (abs(qt_TexCoord0.x - 0.5) < maskWidth / 2.0 && qt_TexCoord0.y > 0.5 ? 0.0 : 1.0);
}
