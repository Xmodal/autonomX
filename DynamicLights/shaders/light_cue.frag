uniform mediump sampler2D source;
varying highp vec2 qt_TexCoord0;

// halo uniforms
uniform vec4 c;
uniform float l;


void main(void)
{
    vec2 st = qt_TexCoord0.st;

    gl_FragColor = mix(texture2D(source, st), c, pow(l * (1.0 - (2.0 * distance(st, vec2(0.5)))), 2.0));
}
