#include <QDebug>
#include <QDir>

#include <QCamResolutionValue>
#include <QCamStillResolution>
#include <QCamVideoResolution>
#include <QCamVideo>
#include <QCamSceneMode>
#include <QCamZoom>
#include <QCamVideoResolution>
#include <QCamWhiteBalance>
#include <QCamColourTone>
#include <QCamEvComp>
#include <QCamSimpleFileNaming>

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
    setupCamDevice();
    connect(ResourceManager::instance(), SIGNAL(resourcesLost()), this, SLOT(resourcesLost()));
}

void Controller::setupCamDevice()
{
    // set the pipeline to video mode
    setVideoMode();

    // setup the viewfinder to the pipeline
    setupViewfinder();

    // setup the stuff needed to store the videos
    setupFileStorage();

    // set the default effect
    setVideoEffect(VIDEO_EFFECT_DEFAULT);
}

void Controller::setVideoMode()
{
    m_device.videoMode()->activate();

    // we need to set here the values for the mode:
    // VideoSceneMode
    QCamSceneMode(&m_device).set(SCENE_MODE_DEFAULT);
    // zoom
    setZoom(ZOOM_DEFAULT);
    // VideoResolution
    setResolution(VIDEO_RESOLUTION_DEFAULT);
    // VideoWBSetting
    QCamWhiteBalance(&m_device).setValue(WHITE_BALANCE_DEFAULT);
    // color filter
    setColorFilter(COLOR_FILTER_DEFAULT);
    // VideoExposureSetting
    QCamEvComp(&m_device).setValue(EV_COMPENSATION_DEFAULT);
    // VideoTorchSetting
    m_device.videoMode()->setTorch(false);
}

void Controller::setupViewfinder()
{
    m_device.setViewfinder(m_viewFinder);
    m_viewFinder->setCamDevice(&m_device);
}

void Controller::setupFileStorage()
{
    //ensure that the storage directory exists, and creat it if needed
    if (!QDir::root().exists(APP_FOLDER)) {
        QDir::root().mkdir(APP_FOLDER);
    }

    // set the file naming pattern
    m_device.setFileNaming(new QCamSimpleFileNaming(APP_FOLDER, INDEX_FILE, &m_device));
}
void Controller::startPipeline()
{
    if (ResourceManager::instance()->acquirePlaybackResources()) {
        m_device.start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    m_device.stop();
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        m_device.videoMode()->startVideoCapture();
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    m_device.videoMode()->stopVideoCapture();
    if (!ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Playback resources denied after recording";
        resourcesLost();
    }
}

void Controller::shutterClicked()
{
    if (m_device.videoMode()->captureState() == QCamVideo::CaptureStopped) {
        startRecording();
        qCritical() << "Controller: recording started";
    } else {
        stopRecording();
        qCritical() << "Controller: recording stopped";
    }
}

void Controller::setResolution(Resolution value)
{
    QCamResolutionValue *res;

    switch (value) {
    case Low:
        // VGA
        res = new QCamResolutionValue(LOW_RES_WIDTH, LOW_RES_HEIGHT, VIDEO_FRN, VIDEO_FRD);
        break;
    case Medium:
        // WVGA
        res = new QCamResolutionValue(MID_RES_WIDTH, MID_RES_HEIGHT, VIDEO_FRN, VIDEO_FRD);
        break;
    case High:
        // 720p
        res = new QCamResolutionValue(HIGH_RES_WIDTH, HIGH_RES_HEIGHT, VIDEO_FRN, VIDEO_FRD);
        break;
    default:
        qCritical() << "Unsupported resolution value " << value;
        return;
    }

    if (QCamVideoResolution(&m_device).setValue(res)) {
        m_currentResolution = value;
    } else {
        qCritical() << "Error setting resolution " << value;
    }
}

void Controller::setZoom(double value)
{
    if (QCamZoom(&m_device).setValue(value)) {
        m_currentZoom = value;
    } else {
        qCritical() << "Error setting zoom value " << value;
    }
}

void Controller::setColorFilter(ColorFilter value)
{
    QCamColourTone::Mode mode;

    switch (value) {
    case Normal:
        mode = QCamColourTone::Normal;
        break;
    case Grayscale:
        mode = QCamColourTone::Grayscale;
        break;
    case Sepia:
        mode = QCamColourTone::Sepia;
        break;
    case Vivid:
        mode = QCamColourTone::Vivid;
        break;
    case Negative:
        mode = QCamColourTone::Negative;
        break;
    case Solarize:
        mode = QCamColourTone::Solarize;
        break;
    default:
        qCritical() << "Unsupported color filter  value " << value;
        return;
    }

    if (QCamColourTone(&m_device).setValue(mode)) {
        m_currentColorFilter = value;
    } else {
        qCritical() << "Error setting color effect " << value;
    }
}

void Controller::setVideoEffect(const QString &value)
{
    CamEffect *cameff = new CamEffect(&m_device, this);
    if (cameff->setValue(EffectManager::instance()->getEffect(value))) {
        m_currentVideoEffect = value;
    } else {
        qCritical() << "Error setting video effect " << value;
    }
}

void Controller::resourcesLost()
{
    qCritical() << "Some resource was lost or denied, stopping recording and pipeline";
    stopRecording();
    stopPipeline();
}
