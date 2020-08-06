// input pixel coordinates
varying highp vec2 coordinates;

// source neuron grid
uniform sampler2D texture;

// uniform props
uniform int     latticeWidthInSquares;
uniform int     latticeHeightInSquares;
uniform float   squareInPixels;
uniform int     containerWidthInPixels;
uniform int     containerHeightInPixels;
uniform vec4    mask;
uniform float   maskAlpha;

// padding between cells, in pixels (defined relative to square size)
float padding = squareInPixels * 0.10;

// container width and height vector, in pixels
ivec2 containerSizeInPixels = ivec2(containerWidthInPixels, containerHeightInPixels);                                               // container size, in pixels
ivec2 latticeSizeInSquares = ivec2(latticeWidthInSquares, latticeHeightInSquares);                                                  // lattice size, in squares
vec2 latticeSizeInPixels = vec2(squareInPixels * float(latticeWidthInSquares), squareInPixels * float(latticeHeightInSquares));     // lattice size, in pixels

vec2 latticeScaleInverse = vec2(containerSizeInPixels) / latticeSizeInPixels;   // inverse of the scale of the lattice relative to the container

vec2 pixelInCentered = vec2(1.0) / vec2(containerSizeInPixels);                 // pixel unit in centered space
vec2 pixelInLattice = pixelInCentered * latticeScaleInverse;                    // pixel unit in lattice space
vec2 squareInLattice = vec2(1.0) / vec2(latticeSizeInSquares);                  // size of a square (including padding) in lattice space

void main() {
    // map NDC coordinates to UV coordinates ([-1, 1] -> [0, 1])
    vec2 coordinatesUnipolar = coordinates * 0.5 + vec2(0.5, 0.5);

    // map UV coordinates to UV coordinates inside lattice
    vec2 coordinatesInLattice = (coordinatesUnipolar * latticeScaleInverse) - ((latticeScaleInverse - 1.0) / 2.0);

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
    vec2 coordinatesInSquare = vec2(mod(coordinatesInLattice.x, squareInLattice.x), mod(coordinatesInLattice.y, squareInLattice.y));

    // hide pixels inside padding
    if(
        !(coordinatesInSquare.x > pixelInLattice.x * padding && coordinatesInSquare.x < pixelInLattice.x * float(latticeWidthInSquares) - (pixelInLattice.x * (padding + 1.0))) ||
        !(coordinatesInSquare.y > pixelInLattice.y * padding && coordinatesInSquare.y < pixelInLattice.y * float(latticeHeightInSquares) - (pixelInLattice.y * (padding + 1.0)))
    ) {
        discard;
    }

    // get floored texture coordinate
    vec2 coordinatesInLatticeFloored = floor(coordinatesInLattice * vec2(latticeSizeInSquares)) / vec2(latticeSizeInSquares) + (0.5 / vec2(latticeSizeInSquares));

    // sample intensity
    float intensity = texture2D(texture, coordinatesInLatticeFloored).r;

    // highlight selected zone if applicable
    // TODO: add float "maskAlpha" - animated in QML
    if (mask.w >= 0.0) {
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
    gl_FragColor = vec4(intensity);
}
