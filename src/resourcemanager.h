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

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <QObject>
#include <policy/resource.h>

class ResourceManager : public QObject {
    Q_OBJECT
public:
    static ResourceManager *instance();
    bool acquirePlaybackResources();
    bool acquireRecordingResources();
    bool releaseResources();

signals:
    void resourcesLost();

private slots:
    void denied();
    void lost();
    void granted();

private:
    ResourceManager(QObject *parent = 0);

    ResourcePolicy::ResourceSet *resources;
    bool result;
    bool waiting;

};
#endif
