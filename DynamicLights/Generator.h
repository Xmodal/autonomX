#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <vector>

class Generator : public QObject
{
    Q_OBJECT
protected:
    // the generator class provides input and output buffers
    std::vector<double> input;
    std::vector<double> output;
public:
    explicit Generator(QObject *parent = nullptr);

    // the method implemented by the derived class that computes the output
    virtual void computeOutput(double deltaTime) = 0;

    // methods to read, write, and query the size of the io buffers
    void writeInput(double value, int index);
    double readOutput(int index);
    int getInputSize();
    int getOutputSize();
};

#endif // GENERATOR_H
