#include "Generator.h"

Generator::Generator(QObject *parent) : QObject(parent)
{

}

void Generator::writeInput(double value, int index) {
    input[index] = value;
}

double Generator::readOutput(int index) {
    return output[index];
}

int Generator::getInputSize() {
    return input.size();
}

int Generator::getOutputSize() {
    return output.size();
}
