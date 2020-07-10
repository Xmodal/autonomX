uniform sampler2D textureMap;
varying highp vec2 qt_TexCoord0;

uniform int cols;
uniform int rows;
uniform int cw;
uniform int ch;

void main(void)
{
    // props
    float pad = 2.0;

    // determine pixel unit relative to GLSL's coord system
    vec2 pxl = (1.0 / cw, 1.0 / ch);
    vec2 cr = vec2(cols, rows);

    // retrieve pixel color and upscale
    vec2 st = floor(qt_TexCoord0.xy * cr) / cr + (0.5 / cr);
    // set to color
    vec4 c = texture2D(textureMap, st);

    // set padding
    vec2 value = vec2(mod(qt_TexCoord0.x, pxl.x*cols), mod(qt_TexCoord0.y, pxl.x*rows));
    c *= value.x > pxl.x*pad && value.x < pxl.x*cols-(pxl.x*(pad+1.0)) ? 1.0 : 0.0;
    c *= value.y > pxl.y*pad && value.y < pxl.y*rows-(pxl.y*(pad+1.0)) ? 1.0 : 0.0;

    // export
    gl_FragColor = c;
}
