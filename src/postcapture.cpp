/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Víctor Jáquez <vjaquez@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <QDBusMessage>
#include <QDBusConnection>
#include <contentaction.h>

#include "common.h"
#include "postcapture.h"

using ContentAction::Action;

PostCapture::PostCapture(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    QDeclarativeComponent
        component(&m_engine, QUrl::fromLocalFile(QML_PATH "PostCapture.qml"));
    QDeclarativeItem *item =
        dynamic_cast<QDeclarativeItem *>(component.create());
    item->setParentItem(this);
    this->setProperty("implicitWidth", item->property("width").toReal());
    this->setProperty("implicitHeight", item->property("height").toReal());
    connect(item, SIGNAL(clicked()), this, SLOT(show()));
}

PostCapture::~PostCapture()
{
}

void PostCapture::show()
{
    emit clicked();

    if (m_file.isEmpty()) {
        qWarning() << "empty file to show";
        return;
    }

    QString uri = m_file;
    uri.prepend("file://");
    qDebug() << Q_FUNC_INFO << uri;

    Action action = Action::defaultActionForFile(uri, "x-maemo-nepomuk/device-captured-video");
    if (action.isValid()) {
        qDebug() << Q_FUNC_INFO << "chosen action:" << action.name();
        action.triggerAndWait();
    } else {
        qWarning() << "could not file action for" << m_file;
    }
}

void PostCapture::setFile(const QString &file)
{
    if (file != m_file) {
        m_file = file;
        emit fileChanged(file);
    }
}
