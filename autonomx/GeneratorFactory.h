//#ifndef GENERATORFACTORY_H
//#define GENERATORFACTORY_H

#include <cassert>
#include <map>
#include <string>
#include <sstream>

#include "Generator.h"
#include "GeneratorType.h"

/**
 * A GeneratorFactory stores stores factories for Generator types.
 * see http://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-us
 */

class GeneratorFactory
{

public:
    typedef std::shared_ptr<GeneratorFactory> ptr;

    bool registerType(const char *name, AbstractGeneratorType::ptr entry);

    template <class T>
    AbstractGeneratorType::ptr registerTypeT(const char *name) {

        AbstractGeneratorType *tmp = (AbstractGeneratorType*) new GeneratorType<T>();
        AbstractGeneratorType::ptr ret(tmp);

        if(! registerType(name, ret)) {
            // add error / throw exception?
        }

        return ret;
    }

    // check that the generator type exists
    bool hasType(const char *name);
    Generator::ptr create(const char *name); //throw exception?

    std::map<std::string, AbstractGeneratorType::ptr> getEntries() const {
        return entries_;
    }

private:
    std::map<std::string, AbstractGeneratorType::ptr> entries_;

};

std::ostream &operator<<(std::ostream &os, const GeneratorFactory &generatorFactory);

//#endif // GENERATORFACTORY_H
