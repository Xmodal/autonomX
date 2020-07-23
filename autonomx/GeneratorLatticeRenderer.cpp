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

void GeneratorLatticeRenderer::render() {
    qDebug() << "render (GeneratorLatticeRenderer)";
    // init shaders on first draw. this isn't really optimal
    if (!program) {
        program = new QOpenGLShaderProgram();
        program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = vertices.xy;"
                                                    "}");
        /*
        program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "lowp float t = 0.5;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                                    "    i = floor(i * 20.) / 20.;"
                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                                    "}");
        */
        program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_FragColor = vec4(coords.x, coords.y, 1.0, 1.0);"
                                                    "}");
        program->bindAttributeLocation("vertices", 0);
        program->link();
    }

    // these don't solve the problem...
    //window->setClearBeforeRendering(false);
    //glDisable(GL_CULL_FACE);

    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    window->beginExternalCommands();

    program->bind();

    program->enableAttributeArray(0);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    // This example relies on (deprecated) client-side pointers for the vertex
    // input. Therefore, we have to make sure no vertex buffer is bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    program->setAttributeArray(0, GL_FLOAT, values, 2);

    glDisable(GL_DEPTH_TEST);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    program->disableAttributeArray(0);
    program->release();

    // these don't do anything
    //glClearColor(1.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);

    window->resetOpenGLState();

    window->endExternalCommands();
}

void GeneratorLatticeRenderer::synchronize(QQuickFramebufferObject *item) {
    // sync with GeneratorLattice
    qDebug() << "synchronize (GeneratorLatticeRenderer)";
    window = item->window();
}
