// input pixel coords
varying highp vec2 qt_TexCoord0;

// source neuron grid
uniform sampler2D textureMap;

// various props
uniform int cols;
uniform int rows;
uniform int ppc;
uniform float cw;
uniform float ch;
uniform vec4 mask;
// uniform float maskAlpha;

void main(void)
{
    // props
    float pad = 3.0;                            // pixel padding between cells
    vec2 s = vec2(ppc*cols, ppc*rows);          // 100% size in pixels
    vec2 wh = vec2(cw, ch);                     // container width/height
    vec2 cr = vec2(cols, rows);                 // cols/rows vector

    vec2 diff = wh / s;                         // offset value to map grid to center
    vec2 pxl = diff / wh;                       // pixel unit relative to GLSL's [0, 1] coord system

    // map tex coord to centered grid
    vec2 st = (qt_TexCoord0 * diff) - ((diff - 1.0) / 2.0);
    // retrieve pixel color and upscale
    vec2 fst = floor(st * cr) / cr + (0.5 / cr);

    // set to color
    vec4 c = vec4(1.0, 1.0, 1.0, 1.0) * texture2D(textureMap, fst).rrrr;
    // hide pixels outside drawing zone
    c *= (st.x < 0.0 || st.y < 0.0 || st.x >= 1.0 || st.y >= 1.0) ? 0.0 : 1.0;

    // highlight selected zone if applicable
    // TODO: add float "maskAlpha" - animated in QML
    if (selected.w >= 0) {
        vec2 selp = mask.xy / cr;
        vec2 seld = mask.zw / cr;
        c *= (st.x < selp.x || st.y < selp.y || st.x >= selp.x + seld.x || st.y >= selp.y + seld.y) ? 0.3 : 1.0;
    }

    // set padding
    vec2 value = vec2(mod(st.x, pxl.x * float(cols)), mod(st.y, pxl.y * float(rows)));
    c *= value.x > pxl.x * pad && value.x < pxl.x * float(cols) - (pxl.x * (pad + 1.0)) ? 1.0 : 0.0;
    c *= value.y > pxl.y * pad && value.y < pxl.y * float(rows) - (pxl.y * (pad + 1.0)) ? 1.0 : 0.0;

    // export color
    gl_FragColor = c;
}
