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

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QCoreApplication>

#include "controller.h"
#include "effectmanager.h"
#include "settings.h"
#include "resourcemanager.h"

Controller::Controller(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_pipeline(0),
      m_recording(false),
      m_pipelineStarting(false),
      m_pipelineReady(false),
      m_recordedTime(0),
      m_recordingTimer(),
      m_currentZoom(ZOOM_DEFAULT),
      m_currentResolution(VIDEO_RESOLUTION_DEFAULT),
      m_currentColorFilter(COLOR_FILTER_DEFAULT),
      m_currentVideoEffect(VIDEO_EFFECT_DEFAULT)
{
    setupEffects();

    connect(ResourceManager::instance(), SIGNAL(resourcesLost()), this, SLOT(resourcesLost()));

    m_pipeline = new Pipeline();
    connect(m_pipeline, SIGNAL(idleChanged(bool)), this, SLOT(idleChanged(bool)));
    connect(m_pipeline, SIGNAL(savedFileNameChanged(const QString &)),
            this, SIGNAL(savedFileNameChanged(const QString &)));
    connect(m_pipeline, SIGNAL(pipelinePlaying()), this, SLOT(pipelineStartingFinished()));

    m_recordingTimer.setInterval(1000);
    connect(&m_recordingTimer, SIGNAL(timeout()), this, SLOT(recordingTimerTimeout()));
    m_pipeline->prepare();
}

Controller::~Controller()
{
    delete m_pipeline;
}

void Controller::startPipeline()
{
    m_pipeline->setWindowId(scene()->views()[0]->effectiveWinId());
    if (ResourceManager::instance()->acquirePlaybackResources()) {
        setPipelineStarting(true);
        QCoreApplication::processEvents();
        m_pipeline->start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    m_pipeline->stop();
    setPipelineReady(false);
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        setRecordedTime(0);
        m_pipeline->startRecording();
        m_recordingTimer.start();
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    m_recordingTimer.stop();
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

void Controller::setColorFilter(ControllerSettings::ColorFilter value)
{
    if (m_currentColorFilter != value) {
        m_currentColorFilter = value;
        m_pipeline->setColorFilter(value);
        emit colorFilterChanged(value);
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
}

void Controller::setPipelineStarting(bool pipelineStarting)
{
    if (m_pipelineStarting != pipelineStarting) {
        m_pipelineStarting = pipelineStarting;
        emit pipelineStartingChanged(m_pipelineStarting);
    }
}

void Controller::pipelineStartingFinished()
{
    setPipelineStarting(false);
    setPipelineReady(true);
}

QString Controller::savedFileName()
{
    return m_pipeline->savedFileName();
}

void Controller::setPipelineReady(bool pipelineReady)
{
    if (m_pipelineReady != pipelineReady) {
        m_pipelineReady = pipelineReady;
        emit pipelineReadyChanged(m_pipelineReady);
    }
}

QString Controller::recordedTime()
{
    QString minutes = QString::number(m_recordedTime/60);
    if (minutes.length() == 1) {
        minutes.prepend("0");
    }
    QString seconds = QString::number(m_recordedTime%60);
    if (seconds.length() == 1) {
        seconds.prepend("0");
    }

    return (minutes + ":" + seconds);
}

void Controller::recordingTimerTimeout()
{
    setRecordedTime(m_recordedTime + 1);
}

void Controller::setRecordedTime(int recordedTime)
{
    if (m_recordedTime != recordedTime) {
        m_recordedTime = recordedTime;
        emit recordedTimeChanged(this->recordedTime());
    }
}
