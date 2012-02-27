#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "controller.h"
// #include "effectmanager.h"
// #include "cameffect.h"
#include "settings.h"
#include "resourcemanager.h"

Controller::Controller(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      recording(false),
      currentZoom(ZOOM_DEFAULT),
      currentResolution(VIDEO_RESOLUTION_DEFAULT),
      currentColorFilter(COLOR_FILTER_DEFAULT),
      currentVideoEffect(VIDEO_EFFECT_DEFAULT)
{
}

void Controller::setup()
{
    setupEffects();
    connect(ResourceManager::instance(), SIGNAL(resourcesLost()), this, SLOT(resourcesLost()));
}

void Controller::startPipeline()
{
    pipeline.setWindowId(scene()->views()[0]->effectiveWinId());
    if (ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Controller: starting pipeline";
        pipeline.start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    qCritical() << "Controller: stopping pipeline";
    pipeline.stop();
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    // EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        qCritical() << "Controller: recording started";
        pipeline.startRecording();
        recording = true;
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    qCritical() << "Controller: recording stopped";
    pipeline.stopRecording();
    recording = false;
    if (!ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Playback resources denied after recording";
        resourcesLost();
    }
}

void Controller::shutterClicked()
{
    if (!recording) {
        startRecording();
    } else {
        stopRecording();
    }
}

void Controller::setResolution(Pipeline::Resolution value)
{
    currentResolution = value;
    pipeline.setResolution(value);
}

void Controller::setZoom(double value)
{
    currentZoom = value;
    pipeline.setZoom(value);
}

void Controller::setColorFilter(Pipeline::ColorFilter value)
{
    currentColorFilter = value;
    pipeline.setColorFilter(value);
}

void Controller::setVideoEffect(const QString &value)
{
    currentVideoEffect = value;
    pipeline.setVideoEffect(value);
}

void Controller::resourcesLost()
{
    qCritical() << "Some resource was lost or denied, stopping recording and pipeline";
    stopRecording();
    stopPipeline();
}
