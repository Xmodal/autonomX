#ifndef COMPUTEENGINE_H
#define COMPUTEENGINE_H

#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include "Generator.h"


class ComputeEngine : public QThread
{
private:
    std::vector<Generator*> generators;
    QElapsedTimer elapsedTimer;
    double frequency = 30;
    bool firstFrame = true;
public:
    ComputeEngine(std::vector<Generator*> generators);
    void run() override;
};

#endif // COMPUTEENGINE_H
