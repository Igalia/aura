/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Simón Pena Placer <spena@igalia.com>
 *          Miguel Gómez <magomez@igalia.com>
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

#include "aboutviewcontroller.h"

#include <QDebug>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QStringList>

static const QString STORE_DBUS_IFACE("com.nokia.OviStoreClient");

AboutViewController::AboutViewController(QObject *parent) : QObject(parent)
{
}

AboutViewController::~AboutViewController()
{
}

void AboutViewController::openStoreClient(const QString &url)
{
    // Based on
    // https://gitorious.org/n9-apps-client/n9-apps-client/blobs/master/daemon/notificationhandler.cpp#line178

    qDebug() << Q_FUNC_INFO << "opening:" << url;

    QDBusInterface dbusInterface(STORE_DBUS_IFACE,
                                 "/",
                                 STORE_DBUS_IFACE,
                                 QDBusConnection::sessionBus());

    QStringList callParams;
    callParams << url;

    dbusInterface.asyncCall("LaunchWithLink", QVariant::fromValue(callParams));
}
