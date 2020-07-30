// Copyright 2020, Xmodal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include "Generator.h"

// multiple inheritance is gross, but needed to allow connections to work
class GeneratorLatticeRenderer : public QQuickFramebufferObject::Renderer, public QObject {
    Q_OBJECT
public:
    GeneratorLatticeRenderer();
    ~GeneratorLatticeRenderer();
    void render();
    void synchronize(QQuickFramebufferObject *item);
private:
    QOpenGLShaderProgram *program;          // pointer to shader
    QQuickWindow *window;                   // pointer to window
    QOpenGLFramebufferObject *framebuffer = nullptr;        // pointer to the assigned framebuffer that is displayed in QML
    QOpenGLFramebufferObject *framebufferSuper = nullptr;   // pointer to high resolution framebuffer that will be downsampled
    QSize size;                             // size of the assigned framebuffer
    QSize sizeSuper;                        // size of the supersampling framebuffer
    int factorSuper = 2;                    // supersampling factor
    bool visible;                           // indicates if the object is visible in QML and turns on and off the render loop accordingly
    bool synchronized = false;              // indicates if there was a call to synchronize since the last render call
    bool synchronizedFirstDone = false;     // indicates if the object was ever synchronized
    bool flagSuper = true;                  // enables supersampling
    bool flagDebug = true;                  // enables debug
    int generatorID;                        // associated generator id
    Generator* generator;                   // associated generator
    double* latticeData = nullptr;          // the lattice data used to draw the graphics
    int* allocatedWidth = nullptr;          // the width of allocated flattened array in the memory block pointed by latticeData
    int* allocatedHeight = nullptr;         // the height of allocated flattened array in the memory block pointed by latticeData
    QMetaObject::Connection connectionWriteLatticeData;         // connection from GeneratorLatticeRenderer's writeLatticeData signal to Generator's writeLatticeData slot
    QMetaObject::Connection connectionWriteLatticeDataCompleted;// connection from Generator's writeLatticeDataCompleted signal to GeneratorLatticeRenderer's writeLatticeDataCompleted slot
    bool writeLatticeDataCurrentDone = true;// indicates if the current writeLatticeData call is done
    bool writeLatticeDataFirstDone = false; // indicates if a the first writeLatticeData call is done, meaning there is valid data to use
signals:
    void writeLatticeData(double* latticeData, int* allocatedWidth, int* allocatedHeight);
public slots:
    void writeLatticeDataCompleted();       // called once writeLatticeData is done, allows a new writeLatticeData signal to be emitted at the end of the next render cycle
};
