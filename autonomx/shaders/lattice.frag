// input pixel coordinates
varying highp vec2 coordinates;

// source neuron grid
uniform sampler2D texture;

// uniform props
uniform float   latticeWidthInSquares;
uniform float   latticeHeightInSquares;
uniform float   squareInPixels;
uniform float   containerWidthInPixels;
uniform float   containerHeightInPixels;
uniform vec4    mask;
uniform float   maskAlpha;

void main() {
    // padding between cells, in pixels (defined relative to square size)
    float padding = squareInPixels * 0.10;

    // container width and height vector, in pixels
    vec2 containerSizeInPixels = vec2(containerWidthInPixels, containerHeightInPixels);                                 // container size, in pixels
    vec2 latticeSizeInSquares = vec2(latticeWidthInSquares, latticeHeightInSquares);                                    // lattice size, in squares
    vec2 latticeSizeInPixels = vec2(squareInPixels * latticeWidthInSquares, squareInPixels * latticeHeightInSquares);   // lattice size, in pixels

    vec2 latticeScaleInverse = containerSizeInPixels / latticeSizeInPixels;     // inverse of the scale of the lattice relative to the container

    vec2 pixelInCentered = vec2(1.0) / containerSizeInPixels;                   // pixel unit in centered space
    vec2 pixelInLattice = pixelInCentered * latticeScaleInverse;                // pixel unit in lattice space
    vec2 squareInLattice = vec2(1.0) / latticeSizeInSquares;                    // size of a square (including padding) in lattice space

    // map NDC coordinates to UV coordinates ([-1, 1] -> [0, 1])
    vec2 coordinatesCentered = coordinates * 0.5 + vec2(0.5, 0.5);

    // map UV coordinates to UV coordinates inside lattice
    vec2 coordinatesInLattice = (coordinatesCentered * latticeScaleInverse) - ((latticeScaleInverse - 1.0) / 2.0);

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
    vec2 coordinatesInLatticeFloored = floor(coordinatesInLattice * latticeSizeInSquares) / latticeSizeInSquares + (0.5 / latticeSizeInSquares);

    // sample intensity
    float intensity = texture2D(texture, coordinatesInLatticeFloored).r;

    // highlight selected zone if applicable
    // TODO: add float "maskAlpha" - animated in QML
    if (mask.w >= 0.0) {
        vec2 maskCornerInLattice = mask.xy / latticeSizeInSquares;   // upper left corner of the mask in lattice space
        vec2 maskSizeInLattice = mask.zw / latticeSizeInSquares;     // width and height of the mask in lattice space
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
