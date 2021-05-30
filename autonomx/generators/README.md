# Generator metadata system
This guide documents the process and necessary steps to expose a metadata chart for a given Generator subclass.

## Preparing the directory
When an insert directive is called on `GeneratorMetaModel`, in `AppModel.cpp`, a single argument is passed: the generator type. **This string corresponds to the name of the sub-directory the ensuing registration process will look for, in the `generators` main directory**. Therefore, you need to make sure that such a folder exists when building the application, and that its name corresponds to the passed string argument.

### Additional .qrc procedures
Currently, the only way to include generators is by burning them into the application when it is built, rather than have them stand as all-in-1 external packages. As such, any new file that is added inside of `generators` should imperatively be included in the Qt resource system file (`qml.qrc`). Errors *will* otherwise occur at runtime.

## Exploring `meta.json` in-depth
Every subfolder that is created to designate a metadata chart for a given generator needs to include a `meta.json` file at its root.

The structure of the enclosed JSON object is as follows:
```json
{
    "name": "My generator",
    "type": "MyGenerator",
    "description": "This is a description for my new generator.",

    "racks": [
        {
            "title": "Rack #1",
            "fields": [
                {
                    "label": "Field #1",
                    "propName": "field1",
                    "type": "number",
                    "default": 10
                    ...
                },
                ...
            ]
        }
    ],
    "help": [
        {
            "title": "Help rack #1",
            "html_path": "help_rack_1.html",
        },
        ...
    ],
    "enumLabels": {
        "MyEnum": [
            "Choice 1",
            "Choice 2",
            "Choice 3",
            "Choice 4",
            ...
        ],
        ...
    }
}
```

Let's run down the properties used here:

### `name`, `type` and `description`
Three required properties that are used for two things:
1) GUI display in the "Generator info" rack (does not concern `type`);
2) proper type attribution in the "Add generator" dropdown (does not concern `description`).

### `racks` property
This property is defined as an array of objects, each representing its own `SubRack` instance inserted in the "Parameters" `Rack`, in QML. Each object, when defined, requires a `"title"` string and an array of `"fields"`.

When defining a new field object, consider the following:
- `label`, `propName` and `type` are **required**;
- `propName` refers to the name of the `Q_PROPERTY` that you've exposed in your `Generator` sub-class beforehand. Make sure that this refers to the *property name*, and not the private member it points to. Also make sure that this property has at least a `READ [method]` declaration in its macro definition, so that it can be effectively read from QML;
- `default` refers to the default value you want this property to initialize at, with its data type reflecting the field type you've previously chosen;
- `type` refers to the field type that should be displayed in the GUI. Other required properties need to be provided depending on which type you choose here (see below for field type options);
- a `flag` property can be provided if you want to allow your parameter to be deactivated. In the `GeneratorMeta` registration process, the associated boolean `Q_PROPERTY` name is understood as **the value of `propName` prefixed by `flag`, camel-cased**. For example, the flag's property name for `"propName": "myProperty"` would be `flagMyProperty`;
- a `flagDefault` boolean property (only necessary if `flag` is set to `true`, to determine) that defines the default value of the flag checkbox.

A static hash map containing the default values for field declarations is defined in `GeneratorMeta::fieldDefaults`.

### `help` property
This property is defined as an array of objects, each representing its own `SubRack` instance inserted at the bottom of the "Generator info" `Rack`. Again, for each object, the `title` and `html_path` properties are required. Make sure the path is relative to the `meta.json` file's parent directory.

The contents of the HTML file follow [QML's StyledText text format standard](https://doc.qt.io/qt-5/qml-qtquick-text.html#textFormat-prop).

### `enumLabels` property
This optional object is used as a string map for `enum`s that might be used both in the C++ `Generator` sub-class and as exposed Qt properties.

For each `enum`, add a key-value pair where:
- the key corresponds to the Pascal-case `enum` name;
- the value corresponds to a string list, of which each element correlates to its respectively ordered `enum` item.

For further information on how to prepare `enum`s for full-stack use, refer to "Additional configuration for `enum`s" below.

## Field type options
Here are the possible values for `type`, followed by the required properties you need to provide for these types:
- `"number"`: int value field; `"min": int, "max": int`;
- `"slider"`: float value field; `"min": float, "max": float`;
- `"select"`: choice selection field; `"enumName": int`;
- `"text"`: single-line text field; `"placeholder": string`;
- `"area"`: multi-line text field; `"placeholder": string`.<

Please note that `select` fields accept an **integer value**, and not a string value; less so an actual enum identifier. This is to facilitate parsing in C++, which is very strict on the ways you can access enum values. When defining a default value for such a field, make sure that the index you input here corresponds to the index of the enum you're trying to target back in C++.

## Tying it all back to C++

In order for these files to be recognized by the software architecture, they need to be explicitly brought into the `GeneratorMetaModel`, as well as have its concurring class types exposed to QML:
- in `main.cpp`, at around L.83, add a call to `qmlRegisterUncreatableType`, passing in the new Generator sub-class as a template argument and its string-ified name as the 4th argument (refer to existing calls);
- in the constructor of `AppModel.cpp`, at L.50, add a call to `insertAtEnd`, passing in the generator type argument previously mentioned;
- in `AppModel::createGenerator`, L.158: add a switch case for your new Generator sub-class, following the pre-existing declarations and replacing appropriate string values by the same generator type argument identifier;
- (optional) unless you're already defining the same default values inline of your header file, **make sure you're calling `resetParameters()` at the end of your constructor**, so that the values inside `meta.json` are effectively applied at first initialization.

### Additional configuration for `enum`s
In the case of `enum` declarations, some extra steps need to be taken. For every new `enum` you bring into the application, we **strongly** recommend you place it inside its own header file, then including it in your .cpp files wherever necessary.

In addition, your enum needs to be encapsulated into its own namespace, with Qt macros registering both the namespace and its enclosed enum. A `typedef` abstracting the namespace enum to an easy-to-use identifier, as well as a call to `Q_DECLARE_METATYPE` on that new identifier, are mandatory.

Here is an example for `NeuronType`:
```cpp
// necessary includes for the ensuing macros
#include <QObject>
#include <QQmlEngine>

namespace NeuronTypeNS {
    Q_NAMESPACE
    enum Enum {
        SpikingNeuron = 0,
        SpikingNeuronRandomized,
        ResonatorNeuron,
        ResonatorNeuronRandomized,
        ChatteringNeuron
    };
    Q_ENUMS(Enum)
}

typedef NeuronTypeNS::Enum NeuronType;
Q_DECLARE_METATYPE(NeuronType);
```

Lastly, you need to register this `enum` to QML. This is currently done in `main.cpp`, at about the same line where you registered your Generator sub-class. Again, here is an example with `NeuronType`:
```cpp
qmlRegisterUncreatableMetaObject(NeuronTypeNS::staticMetaObject, "ca.hexagram.xmodal.autonomx", 1, 0, "NeuronType", "Cannot instanciate NeuronType.");
```

Now, your `enum` should have no trouble getting properly read as a Qt property by QML, as well as being usable as a regular `enum` structure inside your C++ code.
