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

#include <QPointer>
#include "resourcemanager.h"
#include <policy/resource-set.h>

static QPointer<ResourceManager> instancePointer;

ResourceManager* ResourceManager::instance()
{
    if(instancePointer.isNull()){
        instancePointer = new ResourceManager();
    }
    return instancePointer;
}

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent),
      resources(0),
      result(false),
      waiting(false)
{
    qRegisterMetaType<ResourcePolicy::ResourceSet*>("ResourcePolicy::ResourceSet*");
    qRegisterMetaType<ResourcePolicy::ResourceType>("ResourcePolicy::ResourceType");
    qRegisterMetaType<QList<ResourcePolicy::ResourceType> >("QList<ResourcePolicy::ResourceType>");

    resources = new ResourcePolicy::ResourceSet(QString::fromAscii("camera"),
                                                this);
    resources->setAlwaysReply();

    // resources needed while not in capture
    resources->addResource(ResourcePolicy::VideoPlaybackType);
    resources->addResource(ResourcePolicy::ScaleButtonType);

    QObject::connect(resources, SIGNAL(resourcesDenied()),
                     this, SLOT(denied()));
    QObject::connect(resources, SIGNAL(lostResources()),
                     this, SLOT(lost()));
    QObject::connect(resources, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)),
                     this, SLOT(granted()));
}

bool ResourceManager::acquirePlaybackResources()
{
    result = false;
    waiting = true;

    // remove the recording resources just in case they were requested
    resources->deleteResource(ResourcePolicy::VideoRecorderType);
    resources->deleteResource(ResourcePolicy::AudioRecorderType);

    resources->update();
    resources->acquire();

    while (waiting) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }

    return result;
}

bool ResourceManager::acquireRecordingResources()
{
    result = false;
    waiting = true;

    resources->addResource(ResourcePolicy::VideoRecorderType);
    resources->addResource(ResourcePolicy::AudioRecorderType);

    resources->update();
    resources->acquire();

    while (waiting) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }

    return result;
}

bool ResourceManager::releaseResources()
{
    resources->release();
    return true;
}

void ResourceManager::denied()
{
    result = false;
    waiting = false;
}

void ResourceManager::lost()
{
    emit resourcesLost();
}

void ResourceManager::granted()
{
    result = true;
    waiting = false;
}
