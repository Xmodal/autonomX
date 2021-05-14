//#ifndef GENERATORTYPE_H
//#define GENERATORTYPE_H

#include "Generator.h"

class AbstractGeneratorType {

public:
    typedef std::shared_ptr<AbstractGeneratorType> ptr;
    virtual Generator *create() = 0;
};

template <class T>
class GeneratorType {

public:
    virtual Generator *create() {
//        GeneratorTypes selected = type;
//        switch(selected) {
//            case 0:
//                T gen = GeneratorTypes::SNN;
//            break;
//        }

        return new T();
    }
};

//#endif // GENERATORTYPE_H
