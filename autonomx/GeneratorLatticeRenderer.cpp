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

#include <QDebug>
#include "GeneratorLatticeRenderer.h"
#include "GeneratorLattice.h"
#include "AppModel.h"

GeneratorLatticeRenderer::GeneratorLatticeRenderer() : QQuickFramebufferObject::Renderer() {
    if(flagDebug) {
        qDebug() << "constructor (GeneratorLatticeRenderer)";
    }


    // init the function set
    functions = QOpenGLContext::currentContext()->functions();
    functions->initializeOpenGLFunctions();

    // init the shader program
    program = new QOpenGLShaderProgram();
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lattice.vert");
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lattice.frag");
    program->bindAttributeLocation("vertices", 0);
    program->link();

    QString log = program->log();
    if(log != "") {
        throw std::runtime_error(QString(QString("constructor (GeneratorLatticeRenderer): Error in shader compilation or linking: ") + log).toUtf8().constData());
    }

    // init communicator
    communicator = new GeneratorLatticeCommunicator();

    // init latticeData
    latticeData = new float*;
    *latticeData = nullptr;

    // init allocated width / height
    allocatedWidth = new int(0);
    allocatedHeight = new int(0);
}

GeneratorLatticeRenderer::~GeneratorLatticeRenderer() {
    if(flagDebug) {
        qDebug() << "destructor (GeneratorLatticeRenderer)\tid = " << QThread::currentThreadId();
    }
    // delete shader program
    delete program;
    // delete communicator
    delete communicator;
    // delete lattice data (inner pointer) if it exists
    if(*latticeData != nullptr) {
        delete *latticeData;
    }
    // delete lattice data (outer pointer)
    delete latticeData;
    // delete supersampling framebuffer if it exists
    if(flagSuper) {
        if(framebufferSuper != nullptr) {
            delete framebufferSuper;
        }
    }
}

void GeneratorLatticeRenderer::render() {
    if(flagDebug) {
        qDebug() << "render (GeneratorLatticeRenderer)\tid = " << QThread::currentThreadId();
    }

    // exit loop if not visible
    if(!visible) {
        if(flagDebug) {
            qDebug() << "render (GeneratorLatticeRenderer): not visible, exiting loop";
        }
        return;
    }

    if(synchronized) {
        // get the current framebuffer, which will have changed if the window was resized
        framebuffer = this->framebufferObject();

        // check to see if the framebuffer size changed
        QSize sizeNew = framebuffer->size();
        if(sizeNew != size) {
            // the size changed
            size = sizeNew;
            // if supersampling is enabled, we need to update the supersampling framebuffer
            if(flagSuper) {
                if(flagDebug) {
                    qDebug() << "render (GeneratorLatticeRenderer): synchronization caused framebuffer size change, updating supersampling framebuffer";
                }
                if(framebufferSuper != nullptr) {
                    // deallocate old supersampling framebuffer if it exists
                    delete framebufferSuper;
                }
                // create a new supersampling framebuffer
                sizeSuper = size * factorSuper;
                framebufferSuper = new QOpenGLFramebufferObject(sizeSuper);
            }
        }

        synchronized = false;
    }

    // only render if generator is valid and lattice data is ready
    if(generator != nullptr && communicator->isFirstRequestDone()) {
        // lock the lattice data mutex since we want to use that data
        generator->lockLatticeDataMutex();

        // Play nice with the RHI. Not strictly needed when the scenegraph uses
        // OpenGL directly.
        window->beginExternalCommands();

        // bind the texture
        GLuint texture;
        functions->glActiveTexture(GL_TEXTURE0);
        functions->glUniform1i(functions->glGetUniformLocation(program->programId(), "texture"), 0);
        functions->glGenTextures(1, &texture);
        functions->glBindTexture(GL_TEXTURE_2D, texture);
        functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, *allocatedWidth, *allocatedHeight, 0, GL_RED, GL_FLOAT, *latticeData);
        functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // bind the standard framebuffer
        framebuffer->bind();

        // clear the standard framebuffer
        functions->glClearColor(0.0, 0.0, 0.0, 0.0);
        functions->glClear(GL_COLOR_BUFFER_BIT);

        if(flagSuper) {
            // unbind standard framebuffer
            framebuffer->release();

            // bind supersampling framebuffer
            framebufferSuper->bind();

            // set viewport size to match supersampling framebuffer
            functions->glViewport(0, 0, sizeSuper.width(), sizeSuper.height());

            // clear the supersampling framebuffer
            functions->glClearColor(0.0, 0.0, 0.0, 0.0);
            functions->glClear(GL_COLOR_BUFFER_BIT);
        }

        // bind shaders
        program->bind();

        // TODO: what does this do
        program->enableAttributeArray(0);

        // simple rectangle covering the whole NDC XY plane
        float values[] = {
            -1, -1,
            1, -1,
            -1, 1,
            1, 1
        };

        // This example relies on (deprecated) client-side pointers for the vertex
        // input. Therefore, we have to make sure no vertex buffer is bound.
        functions->glBindBuffer(GL_ARRAY_BUFFER, 0);

        // TODO: what does this do
        program->setAttributeArray(0, GL_FLOAT, values, 2);

        // disable depth test
        functions->glDisable(GL_DEPTH_TEST);

        // enable blending
        functions->glEnable(GL_BLEND);
        functions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // update uniforms

        if(flagSuper) {
            program->setUniformValue("squareInPixels", squareInPixels * factorSuper);
            program->setUniformValue("containerWidthInPixels", sizeSuper.width());
            program->setUniformValue("containerHeightInPixels", sizeSuper.height());
        } else {
            program->setUniformValue("squareInPixels", squareInPixels);
            program->setUniformValue("containerWidthInPixels", size.width());
            program->setUniformValue("containerHeightInPixels", size.height());
        }

        program->setUniformValue("mask", mask);
        program->setUniformValue("maskAlpha", maskAlpha);

        program->setUniformValue("latticeWidthInSquares", *allocatedWidth);
        program->setUniformValue("latticeHeightInSquares", *allocatedHeight);


        // draw
        functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // we can now release the lattice data mutex
        generator->unlockLatticeDataMutex();

        // we request an update of the lattice data (as soon as possible!) if the last request finished before this frame
        if(communicator->isCurrentRequestDone()) {
            communicator->writeLatticeData(latticeData, allocatedWidth, allocatedHeight);
        }

        // TODO: what does this do
        program->disableAttributeArray(0);

        // unbind shaders
        program->release();

        if(flagSuper) {
            // release supersampling framebuffer
            framebufferSuper->release();

            // blit supersampling framebuffer onto real framebuffer, performing downsampling
            QOpenGLFramebufferObject::blitFramebuffer(framebuffer, QRect(0, 0, size.width(), size.height()), framebufferSuper, QRect(0, 0, sizeSuper.width(), sizeSuper.height()), GL_COLOR_BUFFER_BIT, GL_LINEAR);
        } else {
            // unbind the standard framebuffer
            framebuffer->release();
        }

        // restore previous OpenGL state
        window->resetOpenGLState();

        // TODO: what does this do
        window->endExternalCommands();
    } else {
        window->beginExternalCommands();

        framebuffer->bind();

        functions->glClearColor(0.0, 0.0, 0.0, 0.0);
        functions->glClear(GL_COLOR_BUFFER_BIT);

        window->resetOpenGLState();

        window->endExternalCommands();
    }

    // render again if visible
    if(visible) {
        update();
    }
}

void GeneratorLatticeRenderer::synchronize(QQuickFramebufferObject *item) {
    // sync with GeneratorLattice
    if(flagDebug) {
        qDebug() << "synchronize (GeneratorLatticeRenderer)";
    }

    GeneratorLattice* generatorLattice = ((GeneratorLattice*) item);

    // remember we are synchronizing now
    synchronized = true;

    bool generatorRefresh = false;

    // check if this is the first synchronization
    if(!synchronizedFirstDone) {
        synchronizedFirstDone = true;
        // update window
        window = item->window();
        // get generator id
        generatorID = generatorLattice->getGeneratorID();
        // mark the generator for a refresh
        generatorRefresh = true;
    } else {
        // check to see if the generator id changed
        int generatorIDNew = generatorLattice->getGeneratorID();
        if(generatorIDNew != generatorID) {
            // update id
            generatorID = generatorIDNew;
            // mark the generator for a refresh
            generatorRefresh = true;
        }
    }

    // update the generator pointer and communicator if needed
    if(generatorRefresh) {
        // update pointer
        generator = AppModel::getInstance().getGenerator(generatorID);

        // check if the generator exists to see if we can update the communicator
        if(generator != nullptr) {

            // update communicator
            communicator->updateGenerator(generator);

            // request lattice data
            communicator->writeLatticeData(latticeData, allocatedWidth, allocatedHeight);
        }
    }

    // update values for uniforms
    squareInPixels = generatorLattice->getSquareInPixels();
    mask = generatorLattice->getMask();
    maskAlpha = generatorLattice->getMaskAlpha();

    // update visible
    visible = item->isVisible();
    if(visible) {
        update();
    }
}
