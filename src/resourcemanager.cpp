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

ResourceManager::ResourceManager()
    : QObject(),
      m_resources(0),
      m_result(false),
      m_waiting(false)
{
    qRegisterMetaType<ResourcePolicy::ResourceSet*>("ResourcePolicy::ResourceSet*");
    qRegisterMetaType<ResourcePolicy::ResourceType>("ResourcePolicy::ResourceType");
    qRegisterMetaType<QList<ResourcePolicy::ResourceType> >("QList<ResourcePolicy::ResourceType>");

    m_resources = new ResourcePolicy::ResourceSet(QString::fromAscii("camera"),
                                                this);
    m_resources->setAlwaysReply();

    // resources needed while not in capture
    m_resources->addResource(ResourcePolicy::VideoPlaybackType);

    QObject::connect(m_resources, SIGNAL(resourcesDenied()),
                     this, SLOT(denied()));
    QObject::connect(m_resources, SIGNAL(lostResources()),
                     this, SLOT(lost()));
    QObject::connect(m_resources,
                     SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)),
                     this, SLOT(granted()));
}

bool ResourceManager::acquirePlaybackResources()
{
    m_result = false;
    m_waiting = true;

    // remove the recording resources just in case they were requested
    m_resources->deleteResource(ResourcePolicy::VideoRecorderType);
    m_resources->deleteResource(ResourcePolicy::AudioRecorderType);

    m_resources->update();
    m_resources->acquire();

    while (m_waiting) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }

    return m_result;
}

bool ResourceManager::acquireRecordingResources()
{
    m_result = false;
    m_waiting = true;

    m_resources->addResource(ResourcePolicy::VideoRecorderType);
    m_resources->addResource(ResourcePolicy::AudioRecorderType);

    m_resources->update();
    m_resources->acquire();

    while (m_waiting) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }

    return m_result;
}

bool ResourceManager::releaseResources()
{
    m_resources->release();
    return true;
}

int ResourceManager::addAudioResources()
{
    int toAdd = 0;

    // The audio resources are optional, because otherwise we would
    // transition from viewfinder to standby if they do not get
    // acquired. We want to return to video viewfinder.

    if (false == m_resources->contains(ResourcePolicy::AudioRecorderType)) {
        ResourcePolicy::AudioRecorderResource *audioRecorderResource =
            new ResourcePolicy::AudioRecorderResource;
        audioRecorderResource->setOptional(true);
        m_resources->addResourceObject(audioRecorderResource);
        ++toAdd;
    }

    return toAdd;
}

int ResourceManager::removeAudioResources()
{
    int toDelete = 0;

    if (m_resources->contains(ResourcePolicy::AudioRecorderType)) {
        m_resources->deleteResource(ResourcePolicy::AudioRecorderType);
        ++toDelete;
    }

    return toDelete;
}

bool ResourceManager::isVideoGranted() const
{
    if (m_resources->resource(ResourcePolicy::VideoRecorderType)->isGranted() &&
        m_resources->resource(ResourcePolicy::VideoPlaybackType)->isGranted()) {
        return true;
    }

    return false;
}

void ResourceManager::denied()
{
    m_result = false;
    m_waiting = false;
}

void ResourceManager::lost()
{
    emit resourcesLost();
}

void ResourceManager::granted()
{
    m_result = true;
    m_waiting = false;
}
