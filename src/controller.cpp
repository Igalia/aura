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

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "controller.h"
#include "effectmanager.h"
#include "settings.h"
#include "resourcemanager.h"

Controller::Controller(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_pipeline(0),
      m_recording(false),
      m_currentZoom(ZOOM_DEFAULT),
      m_currentResolution(VIDEO_RESOLUTION_DEFAULT),
      m_currentColorFilter(COLOR_FILTER_DEFAULT),
      m_currentVideoEffect(VIDEO_EFFECT_DEFAULT)
{
    setupEffects();
    m_pipeline = new Pipeline();

    connect(ResourceManager::instance(), SIGNAL(resourcesLost()), this, SLOT(resourcesLost()));
    connect(m_pipeline, SIGNAL(idleChanged(bool)), this, SLOT(idleChanged(bool)));
}

Controller::~Controller()
{
    delete m_pipeline;
}

void Controller::startPipeline()
{
    m_pipeline->setWindowId(scene()->views()[0]->effectiveWinId());
    if (ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Controller: starting pipeline";
        m_pipeline->start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    qCritical() << "Controller: stopping pipeline";
    m_pipeline->stop();
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        m_pipeline->startRecording();
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    m_pipeline->stopRecording();
    if (!ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Playback resources denied after recording";
        resourcesLost();
    }
}

void Controller::shutterClicked()
{
    if (!m_recording) {
        startRecording();
    } else {
        stopRecording();
    }
}

void Controller::setResolution(Pipeline::Resolution value)
{
    if (m_currentResolution != value) {
        m_currentResolution = value;
        m_pipeline->setResolution(value);
    }
}

void Controller::setZoom(double value)
{
    if (m_currentZoom != value) {
        m_currentZoom = value;
        m_pipeline->setZoom(value);
    }
}

void Controller::setColorFilter(Pipeline::ColorFilter value)
{
    if (m_currentColorFilter != value) {
        m_currentColorFilter = value;
        m_pipeline->setColorFilter(value);
    }
}

void Controller::setVideoEffect(const QString &value)
{
    if (m_currentVideoEffect != value) {
        qDebug() << "setting video effect" << value;
        m_currentVideoEffect = value;
        m_pipeline->setVideoEffect(value);
        emit videoEffectChanged(value);
    }
}

void Controller::resourcesLost()
{
    qCritical() << "Some resource was lost or denied, stopping recording and pipeline";
    stopRecording();
    stopPipeline();
}

void Controller::setRecording(bool recording)
{
    bool emitSignal = m_recording != recording;
    m_recording = recording;
    if (emitSignal) {
        emit recordingChanged(recording);
    }
};

void Controller::idleChanged(bool isIdle)
{
    setRecording(!isIdle);
    if (m_recording) {
        qCritical() << "Controller: recording has started";
    } else {
        qCritical() << "Controller: recording has stopped";
    }
}
