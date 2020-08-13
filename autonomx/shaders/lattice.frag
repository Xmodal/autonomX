// input pixel coordinates
varying highp vec2 coordinates;

// source neuron grid
uniform sampler2D texture;

// uniform props
uniform int     latticeWidthInSquares;
uniform int     latticeHeightInSquares;
uniform float   squareInPixels;
uniform float   containerWidthInPixels;
uniform float   containerHeightInPixels;
uniform vec4    mask;
uniform float   maskAlpha;

// padding between cells, in pixels
float padding = 2.0;

// container width and height vector, in pixels
vec2 containerSizeInPixels = vec2(containerWidthInPixels, containerHeightInPixels);                                                 // container size, in pixels
ivec2 latticeSizeInSquares = ivec2(latticeWidthInSquares, latticeHeightInSquares);                                                  // lattice size, in squares
vec2 latticeSizeInPixels = vec2(squareInPixels * float(latticeWidthInSquares), squareInPixels * float(latticeHeightInSquares));     // lattice size, in pixels

vec2 latticeScaleInverse = containerSizeInPixels / latticeSizeInPixels;     // inverse of the scale of the lattice relative to the container

vec2 pixelInCentered = vec2(1.0) / containerSizeInPixels;                   // pixel unit in centered space
vec2 pixelInLattice = pixelInCentered * latticeScaleInverse;                // pixel unit in lattice space
vec2 squareInLattice = vec2(1.0) / vec2(latticeSizeInSquares);              // size of a square (including padding) in lattice space

void main() {
    // map to coordinates inside lattice
    vec2 coordinatesInLattice = (coordinates * latticeScaleInverse) * 0.5 + 0.5;

    // hide pixels outside lattice
    if(
        coordinatesInLattice.x < 0.0 ||
        coordinatesInLattice.y < 0.0 ||
        coordinatesInLattice.x >= 1.0 ||
        coordinatesInLattice.y >= 1.0
    ) {
        discard;
    }

    // compute location of the pixel in one of the lattice's square, in range [0, 1]
    vec2 coordinatesInSquare = vec2(mod(coordinatesInLattice.x, squareInLattice.x), mod(coordinatesInLattice.y, squareInLattice.y)) / squareInLattice;

    float paddingInSquare = padding / squareInPixels;

    /*
    gl_FragColor = vec4(coordinatesInSquare, coordinatesInSquare.x < 1.0 - paddingInSquare ? 0.0 : 1.0, 1.0);
    return;
    */

    // hide pixels inside padding
    if(
        !(coordinatesInSquare.x > paddingInSquare * 0.5 && coordinatesInSquare.x < (1.0 - paddingInSquare * 0.5)) ||
        !(coordinatesInSquare.y > paddingInSquare * 0.5 && coordinatesInSquare.y < (1.0 - paddingInSquare * 0.5))
    ) {
        gl_FragColor = vec4(0.0);
        return;
    }

    // get floored texture coordinate
    vec2 coordinatesInLatticeFloored = floor(coordinatesInLattice * vec2(latticeSizeInSquares)) / vec2(latticeSizeInSquares) + (0.5 / vec2(latticeSizeInSquares));

    // sample intensity
    float intensity = texture2D(texture, coordinatesInLatticeFloored).r;

    // highlight selected zone if applicable
    if (mask.w > 0.0) {
        vec2 maskCornerInLattice = mask.xy / vec2(latticeSizeInSquares);   // upper left corner of the mask in lattice space
        vec2 maskSizeInLattice = mask.zw / vec2(latticeSizeInSquares);     // width and height of the mask in lattice space
        intensity *= (
            coordinatesInLattice.x < maskCornerInLattice.x ||
            coordinatesInLattice.y < maskCornerInLattice.y ||
            coordinatesInLattice.x >= maskCornerInLattice.x + maskSizeInLattice.x ||
            coordinatesInLattice.y >= maskCornerInLattice.y + maskSizeInLattice.y
        ) ? maskAlpha : 1.0;
    }

    // export color
    gl_FragColor = vec4(vec3(intensity), 1.0);
}
