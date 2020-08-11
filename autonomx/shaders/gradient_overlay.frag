uniform sampler2D source;
varying vec2 qt_TexCoord0;

uniform vec4 overlayColor;
uniform float minAlpha;
uniform float maxAlpha;
uniform bool vertical;

float map(float input, float inputRangeMin, float inputRangeMax, float outputRangeMin, float outputRangeMax) {
    return outputRangeMin + (input - inputRangeMin) * (outputRangeMax - outputRangeMin) / (inputRangeMax - inputRangeMin);
}

void main(void)
{
    vec2 st = qt_TexCoord0;
    gl_FragColor = mix(texture2D(source, st), overlayColor, map((vertical ? st.y : st.x), 0.0, 1.0, minAlpha, maxAlpha));
}
