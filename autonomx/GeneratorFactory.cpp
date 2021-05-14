#include <cassert>
#include <map>
#include <string>
#include <sstream>


#include "GeneratorFactory.h"
#include "Generator.h"


bool GeneratorFactory::registerType(const char *name, AbstractGeneratorType::ptr entry) {

    if(hasType(name)) {
//        std::ostringstream os;
        // duplicate name not accepted
        // error, etc.
        // try again, etc.
//        os << "NodeFactory::" << __FUNCTION__ << "Already got an entry named " << name;
//        Logger::log(ERROR, os.str().c_str());
        return false;
    }

    if(entry.get() == 0) {
//        std::ostringstream os;
        // null pointer exception
        // log error, etc.
//        os << "NodeFactory::" << __FUNCTION__ << ": " << name << " is a " << "null pointer!";
//        Logger::log(ERROR, os.str().c_str());
        return false;
    }

    // do we need this?
//    if (Logger::isEnabledFor(DEBUG))
//    {
//        std::ostringstream os;
//        os << "NodeFactory::" << __FUNCTION__ << "(" << name << ", " << entry << ")";
//        Logger::log(DEBUG, os.str().c_str());
//    }

    entries_[std::string(name)] = entry;
    return true;
}

Generator::ptr GeneratorFactory::create(const char *name) // throw UnknownGeneratorTypeException?
{
    // TODO: if type is not found / cannot be created, throw error or create default?
    // TODO: determine how user-defined generators will be handled / distinguished here

//    if(! hasType(name)) {
//        std::ostringstream os;
//        // throw exception, etc.
////        os << "NodeFactory::" << __FUNCTION__ << ": No such node type: " << name;
////        throw BadNodeTypeException(os.str().c_str());
//    }

    std::string nameString(name);
    AbstractGeneratorType *generatorType = entries_[nameString].get();

    Generator::ptr generator(generatorType->create());
    // does this work?? defined as writeType(QString);
    generator->writeType(name);
    return generator;

}

// Does this check if type of generator trying to be created is a valid type?
// if not, create separate function to test this
bool GeneratorFactory::hasType(const char *name) {
    bool ret = false;

    std::string nameStr(name);

    std::map<std::string, AbstractGeneratorType::ptr>::iterator iter;

    for(iter = entries_.begin(); iter != entries_.end(); ++iter) {
        if((*iter).first == name) {
            ret = true;
            break;
        }
    }
    return ret;
}

// TODO: type check the generator as it's being created


// Do we need this?
//std::ostream &operator<<(std::ostream &os, const GeneratorFactory &generatorFactory) {
//    os << "GeneratorFactory:" << std:: endl;
//    std::map<std::string, AbstractGeneratorType::ptr> entries = generatorFactory.getEntries();
//    std::map<std::string, AbstractGeneratorType::ptr>::iterator iter;

//    for(iter = entries.begin(); iter != entries.end(); ++ iter) {
//        os << " * " << (*iter).first << ": " << (*iter).second.get() << std::endl;
//    }

//    return os;
//}
