#include <QDebug>

#include "controller.h"
#include "effectmanager.h"
#include "cameffect.h"
#include "settings.h"
#include "resourcemanager.h"

Controller::Controller(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_currentZoom(ZOOM_DEFAULT),
      m_currentResolution(VIDEO_RESOLUTION_DEFAULT),
      m_currentColorFilter(COLOR_FILTER_DEFAULT),
      m_currentVideoEffect(VIDEO_EFFECT_DEFAULT)
{
}

void Controller::setup()
{
    setupEffects();

    // initialize QCamDevice
    setupPipeline();
    connect(ResourceManager::instance(), SIGNAL(resourcesLost()), this, SLOT(resourcesLost()));
}

void Controller::setupPipeline()
{
    // set the pipeline to video mode
    setVideoMode();

    // set the default effect
    setVideoEffect(VIDEO_EFFECT_DEFAULT);
}

void Controller::setVideoMode()
{
    pipeline.setVideoMode();
}

void Controller::startPipeline()
{
    //code to get the winId
    // int winId = 0;

    // QList<QGraphicsView*> viewsList = scene()->views();

    // if (viewsList.size() == 1) {
    //     winId = viewsList[0]->effectiveWinId();
    // } else {
    //     qCritical() << "there is " << viewsList.size() << "views";
    // }

    if (ResourceManager::instance()->acquirePlaybackResources()) {
        pipeline.start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    pipeline.stop();
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        pipeline.startRecording();
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    pipeline.stopRecording();
    if (!ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Playback resources denied after recording";
        resourcesLost();
    }
}

void Controller::shutterClicked()
{
    // if (m_device.videoMode()->captureState() == QCamVideo::CaptureStopped) {
    //     startRecording();
    //     qCritical() << "Controller: recording started";
    // } else {
    //     stopRecording();
    //     qCritical() << "Controller: recording stopped";
    // }
}

void Controller::setResolution(Pipeline::Resolution value)
{
    pipeline.setResolution(value);
}

void Controller::setZoom(double value)
{
    pipeline.setZoom(value);
}

void Controller::setColorFilter(Pipeline::ColorFilter value)
{
    pipeline.setColorFilter(value);
}

void Controller::setVideoEffect(const QString &value)
{
    pipeline.setVideoEffect(value);
}

void Controller::resourcesLost()
{
    qCritical() << "Some resource was lost or denied, stopping recording and pipeline";
    stopRecording();
    stopPipeline();
}
