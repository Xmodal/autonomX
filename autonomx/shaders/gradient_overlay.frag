uniform sampler2D source;
varying vec4 qt_TexCoord0;

uniform vec4 overlayColor;
uniform float minAlpha;
uniform float maxAlpha;
uniform bool vertical;

float map(float value, float l1, float h1, float l2, float h2) {
    return l2 + (value - l1) * (h2 - l2) / (h1 - l1);
}

void main(void)
{
    vec2 st = qt_TexCoord0.st;
    gl_FragColor = mix(texture2D(source, st), overlayColor, map((vertical ? st.y : st.x), 0.0, 1.0, minAlpha, maxAlpha));
}
