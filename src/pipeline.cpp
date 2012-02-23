#include <QDebug>
#include <QDir>
#include "pipeline.h"

Pipeline::Pipeline(QObject *parent)
    : QObject(parent)
{
}

void Pipeline::setVideoMode()
{
}
void Pipeline::setupFileStorage()
{
    //ensure that the storage directory exists, and creat it if needed
    if (!QDir::root().exists(APP_FOLDER)) {
        QDir::root().mkdir(APP_FOLDER);
    }

    // // set the file naming pattern
    // m_device.setFileNaming(new QCamSimpleFileNaming(APP_FOLDER, INDEX_FILE, &m_device));
}
void Pipeline::start()
{
}

void Pipeline::stop()
{
}

void Pipeline::startRecording()
{
}

void Pipeline::stopRecording()
{
}

void Pipeline::setResolution(Resolution value)
{
    switch (value) {
    case Low:
        // VGA
        break;
    case Medium:
        // WVGA
        break;
    case High:
        // 720p
        break;
    default:
        qCritical() << "Unsupported resolution value " << value;
        return;
    }
}

void Pipeline::setZoom(double value)
{
}

void Pipeline::setColorFilter(ColorFilter value)
{
    switch (value) {
    case Normal:
        break;
    case Grayscale:
        break;
    case Sepia:
        break;
    case Vivid:
        break;
    case Negative:
        break;
    case Solarize:
        break;
    default:
        qCritical() << "Unsupported color filter  value " << value;
        return;
    }
}

void Pipeline::setVideoEffect(const QString &value)
{
    // CamEffect *cameff = new CamEffect(&m_device, this);
    // if (cameff->setValue(EffectManager::instance()->getEffect(value))) {
    //     m_currentVideoEffect = value;
    // } else {
    //     qCritical() << "Error setting video effect " << value;
    // }
}
