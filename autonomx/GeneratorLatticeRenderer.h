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

#include <QQuickFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QSharedPointer>

#include "Generator.h"
#include "GeneratorLatticeCommunicator.h"

class GeneratorLatticeRenderer : public QQuickFramebufferObject::Renderer {
public:
    GeneratorLatticeRenderer();
    ~GeneratorLatticeRenderer();
    void render();
    void synchronize(QQuickFramebufferObject *item);
private:
    QOpenGLFunctions* functions;            // pointer to OpenGL function set
    QOpenGLShaderProgram *program;          // pointer to shader
    QQuickWindow *window;                   // pointer to window
    QOpenGLFramebufferObject *framebuffer = nullptr;        // pointer to the assigned framebuffer that is displayed in QML
    QOpenGLFramebufferObject *framebufferSuper = nullptr;   // pointer to high resolution framebuffer that will be downsampled
    GLuint texture;
    QSize size;                             // size of the assigned framebuffer
    QSize sizeSuper;                        // size of the supersampling framebuffer
    int factorSuper = 2;                    // supersampling factor
    bool visible;                           // indicates if the object is visible in QML and turns on and off the render loop accordingly
    bool synchronized = false;              // indicates if there was a call to synchronize since the last render call
    bool synchronizedFirstDone = false;     // indicates if the object was ever synchronized
    bool flagSuper = true;                  // enables supersampling
    bool flagDebug = false;                 // enables debug
    int generatorID;                        // associated generator id
    QSharedPointer<Generator> generator;    // associated generator
    GeneratorLatticeCommunicator communicator;
    float** latticeData;        // the lattice data used to draw the graphics
    int* allocatedWidth;        // the width of allocated flattened array in the memory block pointed by latticeData
    int* allocatedHeight;       // the height of allocated flattened array in the memory block pointed by latticeData
    float squareInPixels;       // uniform for pixel size of a lattice square
    QVector4D mask;             // uniform for mask shape (leftmost x, topmost y, width, height)
    float maskAlpha;            // uniform for mask alpha
};
