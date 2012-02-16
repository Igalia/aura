#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QtDeclarative>
#include <QDir>

#include <MScene>

#include <QDebug>

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
#include "xvviewfinder.h"
#include "effectmanager.h"
#include "cameffect.h"
#include "settings.h"
#include "resourcemanager.h"

Controller::Controller(QObject *parent)
    : QObject(parent),
      mainWindow(),
      device(),
      currentZoom(ZOOM_DEFAULT),
      currentResolution(VIDEO_RESOLUTION_DEFAULT),
      currentColorFilter(COLOR_FILTER_DEFAULT),
      currentVideoEffect(VIDEO_EFFECT_DEFAULT)
{
    mainWindow.setLandscapeOrientation();
    mainWindow.setOrientationAngleLocked(true);
    mainWindow.showFullScreen();

    // load the available effects
    setupEffects();

    // initialize QCamDevice
    setupCamDevice();

    connect(&mainWindow, SIGNAL(displayEntered()), this, SLOT(startPipeline()));
    connect(&mainWindow, SIGNAL(displayExited()), this, SLOT(stopPipeline()));
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
    device.videoMode()->activate();

    // we need to set here the values for the mode:
    // VideoSceneMode
    QCamSceneMode(&device).set(SCENE_MODE_DEFAULT);
    // zoom
    setZoom(ZOOM_DEFAULT);
    // VideoResolution
    setResolution(VIDEO_RESOLUTION_DEFAULT);
    // VideoWBSetting
    QCamWhiteBalance(&device).setValue(WHITE_BALANCE_DEFAULT);
    // color filter
    setColorFilter(COLOR_FILTER_DEFAULT);
    // VideoExposureSetting
    QCamEvComp(&device).setValue(EV_COMPENSATION_DEFAULT);
    // VideoTorchSetting
    device.videoMode()->setTorch(false);
}

void Controller::setupViewfinder()
{
    XvViewFinder *viewfinder = new XvViewFinder(&device);

    viewfinder->setMinimumSize(mainWindow.size());
    viewfinder->setMaximumSize(mainWindow.size());
    viewfinder->setZValue(-1.0);
    mainWindow.scene()->addItem(viewfinder);
    mainWindow.ensureVisible(viewfinder);
    viewfinder->setPos(0, 0);

    device.setViewfinder(viewfinder);

    connect(viewfinder, SIGNAL(mouseReleased()), this, SLOT(displayClicked()));
}

void Controller::setupFileStorage()
{
    //ensure that the storage directory exists, and creat it if needed
    if (!QDir::root().exists(APP_FOLDER)) {
        QDir::root().mkdir(APP_FOLDER);
    }

    // set the file naming pattern
    device.setFileNaming(new QCamSimpleFileNaming(APP_FOLDER, INDEX_FILE, &device));
}
void Controller::startPipeline()
{
    if (ResourceManager::instance()->acquirePlaybackResources()) {
        device.start();
    } else {
        qCritical() << "Playback resources denied";
        resourcesLost();
    }
}

void Controller::stopPipeline()
{
    device.stop();
    ResourceManager::instance()->releaseResources();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}

void Controller::startRecording()
{
    if (ResourceManager::instance()->acquireRecordingResources()) {
        device.videoMode()->startVideoCapture();
    } else {
        qCritical() << "Recording resources denied";
        resourcesLost();
    }
}

void Controller::stopRecording()
{
    device.videoMode()->stopVideoCapture();
    if (!ResourceManager::instance()->acquirePlaybackResources()) {
        qCritical() << "Playback resources denied after recording";
        resourcesLost();
    }
}

void Controller::displayClicked()
{
    if (device.videoMode()->captureState() == QCamVideo::CaptureStopped) {
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

    if (QCamVideoResolution(&device).setValue(res)) {
        currentResolution = value;
    } else {
        qCritical() << "Error setting resolution " << value;
    }
}

void Controller::setZoom(double value)
{
    if (QCamZoom(&device).setValue(value)) {
        currentZoom = value;
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

    if (QCamColourTone(&device).setValue(mode)) {
        currentColorFilter = value;
    } else {
        qCritical() << "Error setting color effect " << value;
    }
}

void Controller::setVideoEffect(const QString &value)
{
    CamEffect *cameff = new CamEffect(&device, this);
    if (cameff->setValue(EffectManager::instance()->getEffect(value))) {
        currentVideoEffect = value;
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
