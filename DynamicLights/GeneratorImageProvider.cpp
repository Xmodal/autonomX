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

#include "GeneratorImageProvider.h"
#include "Generator.h" // here because of forward declaration

GeneratorImageProvider::GeneratorImageProvider(Generator* generator) : QQuickImageProvider(QQuickImageProvider::Pixmap) {
    this->generator = generator;
}

QPixmap GeneratorImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    if(id == "lattice") {
        QSize imageSize(generator->getLatticeWidth(), generator->getLatticeHeight());
        //QImage image(imageSize, QImage::Format_Grayscale8);
        QImage image(imageSize, QImage::Format_RGB32);
        for(int x = 0; x < generator->getLatticeWidth(); x++) {
            for(int y = 0; y < generator->getLatticeHeight(); y++) {
                QColor pixelColor = generator->getLatticeAt(x, y);
                qDebug() << "(" << x << ", " << y << ") = " << pixelColor;
                image.setPixelColor(x, y, pixelColor);
            }
        }
        return QPixmap::fromImage(image);
    } else {
        return QPixmap();
    }
}
