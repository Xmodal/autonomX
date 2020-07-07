uniform sampler2D textureMap;
varying vec4 qt_TexCoord0;

uniform int imgWidth;
uniform int imgHeight;

void main(void)
{
    vec2 st = floor(qt_TexCoord0.xy * imgWidth) / imgWidth + (0.5 / imgWidth);
    gl_FragColor = texture2D(textureMap, st);
}
