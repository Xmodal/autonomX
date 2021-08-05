# Implementing a custom generator in C++

## What is a "generator"?
Simply put, a generator is:
- a grid-based wrapper for an AI and/or ALife algorithm;
- represented by a 2D pixel lattice populated by cells;
- sensitive to external stimuli through input regions;
- interpretable into multiple targeted output values.

## Mandatory methods

There are **four methods** to override from the base `Generator` class:
1. `initialize()`: this method (re)initializes the generator. This method is called when creating a new generator instance from the "Add" dropdown and when clicking on the "Restart" button in the upper toolbar; it can also be called after modifying a generator parameter that fundamentally changes your cells' state and/or order (for instance, changing the lattice dimensions through `latticeWidth` and `latticeHeight` requires a clean slate);
2. `computeIteration(double deltaTime)`: this method is called by `ComputeEngine` at a steady 60fps rate (mediated through the `deltaTime` argument). **This usually contains the core of your algorithm**. Furthermore, this is where you can alter the refresh rate of your system in whichever way is best suited to your algorithm through the generic `speed` parameter;
3. `getLatticeValue(int x, int y)`: this method retrieves the value of a single cell located at (x, y) **The value that is returned here directly correlates to the intensity of the corresponding cell in the GUI representation of the grid, and it must range between 0 (completely off) and 1 (completely on)**. As such, if you are working with "cell-objects" with non-normalized values (as is the case for the SNN), this is where you may want to remap its value;
4. `writeLatticeValue(int x, int y, double value)`: this method writes a value passed as an argument to a single cell located at (x, y). This method is called by the compute engine when receiving a correctly-formatted OSC message, for all cells that the represented GUI input region contains. Again, **the `value` argument always ranges between 0 and 1**, so be cautious if your algorithm uses different value ranges.

## Template notes

1. the cell values are grouped into a 1D vector array of `double`s, and all cell operations are done to that vector;
2. `initialize` resets the cells array to a `(latticeWidth * latticeSize)` size;
3. all operations involving 2D coordinates remap the (x, y) values to a flattened 1D index;
4. `computeIteration` implements a simple timer that scales the execution rate to the `speed` factor (a similar strategy is employed in WolframCA and GameOfLife).