# Release Notes for autonomX

## Version 1.0.0 - 2021-10-21

This is a major release of AutonomX!
We introduce multiple new features, including state saving and the support
of multiple generators.

New features: (most recent first)

- Port the example Max patch to the new OSC format
- Format OSC output in the form /generatorName/output(s)
- Allow to send data to the generators via the /input and /inputs OSC paths
- Add the possibility to control generator parameters via OSC
- Add a reset button
- Add the GitHub repo link to the OscSettings component
- Add some scripts to bundle the macOS app
- Add note about Ctrl and Cmd mappings; change view reset shortcut to Ctrl+0
- Improve focus state styles on fields
- Add pan/zoom
- Send OSC to only one port #303
- Allow to fine-tune the speed of each generators
- Implement state saving
- Add support for multiple generators
- Generally improve key event handling
- Can now reset a region
- Can now add and remove regions
- Use C++ 17 (#308)
- Implement save/load functionalities (#285)
- Document the architecture (#266)
- Integrate the metadata for each generator to the GUI
- Add a metadata folder for each generator type
- Add the GameOfLife and WolframCA generators
- Add a restart and a reset button
- Add a OSC.md documentation file
- Implement alt-dragging with shift finetune for Slider fields
- Redesign of most of the user interface
- Add a Doxyfile (#244)
- Add an example Max MSP patch


## Version 0.1.0 - 2020-09-12

This is the first release of autonomX.

New features related to generators:

- See the list of all available generators
- Create a new generator
- Change the type of a generator
- Edit the properties of a generator
- Run a generator
- Delete a generator
- Implement the SNN generator type

New features related to sending and receiving OSC:

- Configure the parameters of the OSC input and output
- Send the outputs of the generator to the outside with OSC
- Receive OSC from the outside and feed it to the inputs of the generator

New features related to routing and visualization:

- Have multiple inputs and outputs
- Draw the inputs and outputs on a 2D lattice representation
- See different colors for each inputs and outputs
- View the current value of the generator output node
- View the evolution of the output node of the generator over time
- See a visual approximation of the value of each region via its opacity

