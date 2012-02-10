#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QtDeclarative>

#include <MScene>

#include <QDebug>

#include <QCamResolutionValue>
#include <QCamStillResolution>
#include <QCamVideoResolution>

#include "controller.h"
#include "xvviewfinder.h"
#include "effectmanager.h"

#define STILL_WIDTH 640
#define STILL_HEIGHT 480
#define STILL_FRN 30
#define STILL_FRD 1

#define VIDEO_WIDTH 848
#define VIDEO_HEIGHT 480
#define VIDEO_FRN 30
#define VIDEO_FRD 1

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    mainWindow.setLandscapeOrientation();
    mainWindow.setOrientationAngleLocked(true);
    mainWindow.showFullScreen();

    setupEffects();
    setupCamDevice();

    connect(&mainWindow, SIGNAL(displayEntered()), this, SLOT(startPipeline()));
    connect(&mainWindow, SIGNAL(displayExited()), this, SLOT(stopPipeline()));
}

void Controller::setupCamDevice()
{
    QCamStillResolution *stillReso = new QCamStillResolution(&device);
    stillReso->setValue(new QCamResolutionValue(STILL_WIDTH, STILL_HEIGHT,
                                                STILL_FRN, STILL_FRD));
    QCamVideoResolution *videoReso = new QCamVideoResolution(&device);
    videoReso->setValue(new QCamResolutionValue(VIDEO_WIDTH, VIDEO_HEIGHT,
                                               VIDEO_FRN, VIDEO_FRD));

    setupViewfinder();
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
}

void Controller::startPipeline()
{
    device.start();
}

void Controller::stopPipeline()
{
    device.stop();
}

void Controller::setupEffects()
{
    EffectManager::setup(this);
}
