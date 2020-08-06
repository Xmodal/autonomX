// input pixel coords
varying highp vec2 coords;

// source neuron grid
uniform sampler2D texture;

// uniform props
uniform float cols;
uniform float rows;
uniform float ppc;
uniform float cw;
uniform float ch;
// uniform vec4 mask;
// uniform float maskAlpha;

void main() {
    // props
    float pad = 3.0;                            // pixel padding between cells
    vec2 s = vec2(ppc*cols, ppc*rows);          // 100% size in pixels
    vec2 wh = vec2(cw, ch);                     // container width/height
    vec2 cr = vec2(cols, rows);                 // cols/rows vector

    vec2 diff = wh / s;                         // offset value to map grid to center
    vec2 pxl = diff / wh;                       // pixel unit relative to GLSL's [0, 1] coord system

    // map tex coord to centered grid
    vec2 st = coords * 0.5 + vec2(0.5, 0.5);
    st = (st * diff) - ((diff - 1.0) / 2.0);

    // hide pixels outside drawing zone
    if(st.x < 0.0 || st.y < 0.0 || st.x >= 1.0 || st.y >= 1.0) {
        discard;
    }

    // set padding
    vec2 value = vec2(mod(st.x, pxl.x * float(cols)), mod(st.y, pxl.y * float(rows)));

    // hide pixels inside padding
    if(!(value.x > pxl.x * pad && value.x < pxl.x * float(cols) - (pxl.x * (pad + 1.0))) ||
       !(value.y > pxl.y * pad && value.y < pxl.y * float(rows) - (pxl.y * (pad + 1.0)))) {
        discard;
    }

    // get floored texture coordinate
    vec2 fst = floor(st * cr) / cr + (0.5 / cr);

    // sample color
    float c = texture2D(texture, fst).r;

    // highlight selected zone if applicable
    // TODO: add float "maskAlpha" - animated in QML
    /*
    if (mask.w >= 0.0) {
        vec2 selp = mask.xy / cr;
        vec2 seld = mask.zw / cr;
        c *= (st.x < selp.x || st.y < selp.y || st.x >= selp.x + seld.x || st.y >= selp.y + seld.y) ? 0.3 : 1.0;
    }
    */

    // export color
    gl_FragColor = vec4(c);
}
