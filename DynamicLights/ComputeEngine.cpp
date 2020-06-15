#include "ComputeEngine.h"
#include "algorithm"
#include <QDebug>

ComputeEngine::ComputeEngine(std::vector<Generator*> generators)
{
    this->generators = generators;
}

void ComputeEngine::run() {
    // this solution is not very robust if CPU resources are in high demand by other programs
    // this breaks also after a while if the application isn't interacted with (ie is not the operating system's currently active app)
    // QTimer seems to have some bugs :(

    double millisCompute;   // time in nanoseconds taken by computation
    double millisLastFrame; // time in nanoseconds since last frame

    // compute time since last frame, with exception if this is the first frame
    if(firstFrame) {
        firstFrame = false;
        millisLastFrame = 1.0 / frequency * 1000.0;
    } else {
        millisLastFrame = elapsedTimer.nsecsElapsed() / 1000000.0;
    }

    // restart the timer to measure frame time and compute time
    elapsedTimer.restart();
    elapsedTimer.start();

    // do the computation
    for(std::vector<Generator*>::iterator it = generators.begin(); it != generators.end(); it++) {
        (*it)->computeOutput(millisLastFrame / 1000.0);
    }

    // measure the time used to do the computation
    millisCompute = elapsedTimer.nsecsElapsed() / 1000000.0;

    qDebug() << "refresh interval: " << millisLastFrame;
    qDebug() << "compute time:     " << millisCompute;

    // schedule a new function call at the appropriate time
    // this bit maybe could be improved?
    // QTimer only guarantees the timing interval is AT LEAST equal to the duration we ask it for.
    // if there are other things using up CPU, this can fail very hard.
    // on my development machine (Simon), it can drop out spectacularly (going suddenly from 30FPS to 0.1FPS)
    QTimer timer;
    timer.singleShot((int) std::min<double>(1.0 / frequency * 1000.0, std::max<double>(1.0 / frequency * 1000.0 - millisCompute, 0)), this, &ComputeEngine::run);
}
