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

#include <QRunnable>
#include <QDebug>
#include "GeneratorLattice.h"

GeneratorLattice::GeneratorLattice() : m_renderer(nullptr) {
    connect(this, &QQuickItem::windowChanged, this, &GeneratorLattice::handleWindowChanged);
}


void GeneratorLattice::handleWindowChanged(QQuickWindow *window) {
    if (window) {
        connect(window, &QQuickWindow::beforeSynchronizing, this, &GeneratorLattice::sync, Qt::DirectConnection);
        connect(window, &QQuickWindow::sceneGraphInvalidated, this, &GeneratorLattice::cleanup, Qt::DirectConnection);
        // Ensure we start with cleared to black. The squircle's blend mode relies on this.
        window->setColor(Qt::black);
    }
}

void GeneratorLattice::sync() {
    if (!m_renderer) {
        m_renderer = new GeneratorLatticeRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &GeneratorLatticeRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &GeneratorLatticeRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setWindow(window());
}

void GeneratorLattice::cleanup() {
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable {
public:
    CleanupJob(GeneratorLatticeRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    GeneratorLatticeRenderer *m_renderer;
};

void GeneratorLattice::releaseResources() {
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}
