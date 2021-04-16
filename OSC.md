#  OSC Documentation

This file contains the documentation of AutonomX's use of [OSC] (http://opensoundcontrol.org/) protocol and related info for implementation.

## General Description

There is one unique **OscEngine** object that is responsible for managing OSC communications.

Additionally, there is one of each of the following objects *per Generator*:

* one **OscReceiver** object
    * Responsible for receiving OSC data from indicated port

* one **OscSender** object
    * Responsible for sending OSC data to indicated port
    
## OSC Configuration
    
These OscReceiver and OscSender objects can be configured on a per-generator basis via the "OSC SETTINGS" tab (IN and OUT) in the parameters section of the generator (right side of the interface).

Each OscReceiver (IN) allows you to specify the incoming address and port number of the incoming messages.

Each OscSender (OUT) allows you to specify the outgoing address, host, and port number of the outgoing messages.

These OSC objects follow traditional OSC protocol formats and guidelines.

## OSC Messages

INPUT OSC messages received by AutonomX (OscReceiver) must be composed of floating point numbers. Each float corresponds to one of the BLUEISH input rectangles of the central lattice.

The OSC input messages are in the form of:

input: /inputAddress <in1>...<inN>

where:
/inputAddress = incoming address to AutonomX
<in1>...<inN> = the 1 to N number of input floats

Example input message, using the default SNN generator: /inputAddress 0.52 0.34 0.24 0.11

OUTPUT OSC messages sent by AutonomX (OscSender) are composed of floating point numbers. Each float corresponds to one of the REDDISH output rectangles of the central lattice.

output: /outputAddress <host> <out1>...<outN>

where:
/outputAddress = outgoing address in external software
<host> = host address
<out1>...<outN> = the 1 to N number of outgoing floats

Example output message, using the default SNN generator: /outputAddress 127.0.0.1 0.12 0.01 0.31 0.22

For more information about how incoming messages are translated into values that affect the visual lattice and, vice versa, how values on the visual lattice are translated to outgoing float values, see each individual generator type's description.
(i.e. an SNN will not resolve incoming floats or generate outgoing floats the same way as a cellular automata generator, etc.)

**Warning:** each generator implemented in AutonomX should have *unique* ports for its OSC input and output. By default, new generators are given the same port numbers and therefore do not send nor receive OSC. Make sure to set unique port numbers for each new generator. (a fix to this issue is in progress)

## OSC Internal Structure

Will descibe message passing system, argument types, generator inputs and outputs here

Any implemented generator in AutonomX (i.e. those listed in the left-hand menu) will be responsible for controlling the OSC settings (via message passing) that correpond to that generator. Each implemented generator will have its own unique OscReceiver and OscSender objects, created by the unique, global OscEngine object, which also sets up their message passing threads.

This global OscEngine object will receive all external INPUT messages from any OscReceiver object and forward them on to the global ComputeEngine that is responsible for scheduling generator calculations for the implemented generator(s).

After receiving the updated values from the generator(s), the ComputeEngine will then forward the OUTPUT messages to the OscEngine, who then communicates the outgoing values to the corresponding OscSend object associated to that generator.

## See Also

[OSC Wikipedia](https://en.wikipedia.org/wiki/Open_Sound_Control)

[Open Sound Control Website](http://opensoundcontrol.org/)
