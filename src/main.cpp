/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
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

#include <QDeclarativeView>
#include <QApplication>
#include <QtDeclarative>

#include <gst/gst.h>

#include "controller.h"
#include "effectmanager.h"
#include "common.h"
#include "pipeline.h"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    QApplication app(argc, argv);

    QDeclarativeView view;
    qmlRegisterType<Controller>("aura.controller", 1, 0, "Controller");
    qmlRegisterUncreatableType<Pipeline::ColorFilter>("aura.pipeline."
                                                      "colorfilters", 1, 0,
                                                      "ColorFilter",
                                                      "Exporting ColorFilter "
                                                      "settings to QML");
    QDeclarativeContext *context = view.rootContext();
    EffectManager::setup();
    context->setContextProperty("effectManager", EffectManager::instance());
    qDebug("opening qml files in %s", QML_PATH);
    view.setSource(QUrl::fromLocalFile(MAIN_QML_FILE));
    view.showFullScreen();

    int ret = app.exec();

    gst_deinit();
    return ret;
}
