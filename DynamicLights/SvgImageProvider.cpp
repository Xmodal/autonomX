// Copyright 2020, Atsushi Masumori & Xmodal
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

#include "SvgImageProvider.h"

#include <QSvgRenderer>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QDebug>

SvgImageProvider::SvgImageProvider() : QQuickImageProvider(QQmlImageProviderBase::ImageType::Image) {}

QImage SvgImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    auto path = QStringLiteral(":/%1.svg").arg(id);
    qDebug() << "attempt to provide image at path: " << path;
    qDebug() << "requested size: " << *size;
    auto renderer = new QSvgRenderer(path);
    if(renderer->isValid()) {
        QSize tSize;
        if(requestedSize.isValid())
            tSize = requestedSize;
        else {
            tSize = renderer->defaultSize();
            tSize *= QGuiApplication::primaryScreen()->devicePixelRatio();
        }
        qDebug() << "served size: " << tSize;

        QImage image(tSize, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        renderer->render(&painter);
        *size = image.size();

        return image;
    } else {
        *size = QSize();
        return QImage();
    }
}
