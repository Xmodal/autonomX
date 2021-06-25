uniform sampler2D source;
varying highp vec2 qt_TexCoord0;
uniform float qt_Opacity;

uniform vec4 srcColor;

float startAlpha = 0.3;
float endAlpha = 0.9;

float map(float inputValue, float inputRangeMin, float inputRangeMax, float outputRangeMin, float outputRangeMax) {
    return outputRangeMin + (inputValue - inputRangeMin) * (outputRangeMax - outputRangeMin) / (inputRangeMax - inputRangeMin);
}

void main(void)
{
    vec2 st = qt_TexCoord0.st;

    gl_FragColor = srcColor * map(st.x, 0.0, 1.0, startAlpha, endAlpha) * qt_Opacity;
}
