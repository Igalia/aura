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

#include <QDeclarativeView>
#include <QApplication>
#include <QtDeclarative>
#include <QtCore/QtGlobal>

#include <glib.h>
#include <gst/gst.h>
#include <string.h>

#include "controller.h"
#include "effectmanager.h"
#include "postcapture.h"

static inline void
env_init()
{
    if (strlen(DATADIR) > 0 && !g_str_has_prefix(DATADIR, "/usr")) {
        g_setenv("XDG_DATA_DIRS",
                 "/usr/local/share/:/usr/share/:" DATADIR,
                 TRUE);
    }
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);
    env_init();

    QApplication app(argc, argv);

    QDeclarativeView view;
    qmlRegisterType<Controller>("aura.controller", 1, 0, "Controller");
    qmlRegisterType<PostCapture>("aura.postcapture", 1, 0, "PostCapture");
    qmlRegisterUncreatableType<ControllerSettings>("aura.controller.settings",
                                                   1, 0, "ControllerSettings",
                                                   "Exporting "
                                                   "ControllerSettings "
                                                   "settings to QML");
    QDeclarativeContext *context = view.rootContext();
    EffectManager::setup();
    context->setContextProperty("effectManager", EffectManager::instance());
    view.setSource(QUrl("qrc:/aura.qml"));
    view.showFullScreen();

    int ret = app.exec();

    gst_deinit();
    return ret;
}
