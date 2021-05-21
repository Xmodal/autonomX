# Notes about the software architecture

The current architecture is kept generic and has the following classes for its functioning:

1. **AppModel**: singleton object containing all main application components. It orchestrates the initialization of engines and threads, and takes care of all operations that involve coordination over multiple threads.
2. **ComputeEngine**: unique object in charge of scheduling Generator calculations and relaying signal input and output to OscEngine.
3. **OscEngine**: unique object in charge of managing OSC communications.
4. **GeneratorModel**: QAbstractListModel subclass that registers all running Generator instances and exposes them to the QML system.
5. **Generator**: generic, *virtual* class that defines all necessary methods, properties and class instances for a generator to function as part of the application's architecture;
6. **GeneratorFacade**: object which mirrors all properties of Generator, allowing QML to read and write parameter values without causing fatal threading issues. One such object exists for each Generator.
7. **GeneratorLattice**: object which allows the embedding of custom graphics code required for visualizing Generator grids as a QML object. This is instanciated from QML. It declares a few QML-editable properties, allowing the targeted generator to be changed. This class does not contain the rendering code itself, but rather wraps it in a QML-interpretable container.
8.  **GeneratorLatticeCommunicator**: object in charge of updating the lattice texture; this involves attaching messages to the correct Generator, requesting the lattice texture buffer to be overwritten by the Generator, and keeping track of query completion.
9.  **GeneratorLatticeRenderer**: object containing the OpenGL graphics code that performs lattice rendering and coordinates lattice texture updating with GeneratorLatticeCommunicator.
10. **GeneratorRegion**: object which represents a single input or output region square.
11. **GeneratorRegionModel**: object which mirrors all properties of GeneratorRegionSet, allowing QML to render Regions in lists and interact with their detailed properties without causing fatal threading issues. Two such objects exist per GeneratorFacade; one dedicated to inputs, the other dedicated to outputs.
12. **GeneratorRegionSet**: object containing a set of input or output regions. Two such objects exist per Generator;
one dedicated to inputs, the other dedicated to outputs.

### SpikingNet exclusive classes

In the current setting there is one generator already implemented in the software - SpikingNet. Hence, two more classes are added into the functionality:

- **SpikingNet**
- **Izhikevich**
- **NeuronType**

## Adding new generators

Any other addition of a generator requires creating a separate C++ class derived from the Generator class as the base class. The current GUI has a lattice (with dynamic user specified width and height) where each cell blinks depending on the input to and output from the choosen generator. Hence, adding any new generator requires sending and receiving the values to the lattice with values specific to the implemented generator.

## Threads on which the application runs

a) mainThread - The main thread runs the QML engine,
Qt’s core services, and all resources
which we directly want to access from
QML.

b) computeThread - The compute thread runs ComputeEngine,
which uses the thread’s Qt event
queue to schedule generator calculations
at the desired update rate with a
timer loop.

c) oscThread - The OSC thread runs OscEngine, which
uses the thread’s Qt event queue to
recieve and send messages.

## A separate Note

One of the main design challenges of using Qt for this application is that QProperties are not thread-safe — they are
cursed to the point of being unusable even through proper mutex safeguarding — we cannot directly use any value
we expose in the UI in a task running on a thread other than the main thread. Here, we absolutely need to separate
UI calculations from backend generator calculations for responsiveness and performance. This problem forced us to
come up with an (unsatisfyingly complex) solution: we need to create facade objects existing on the main thread
that mirror changes onto the true objects used in other threads via Qt’s thread-safe messaging system.


