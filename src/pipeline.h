#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>
#include "settings.h"
#include "gst/gst.h"
#include <gst/interfaces/photography.h>
#include <gst/interfaces/xoverlay.h>
#include <gst/video/video.h>

class Pipeline : public QObject
{
    Q_OBJECT

 public:
    // resolution values
    typedef enum {
        Low = 0,
        Medium,
        High
    } Resolution;

    // colour filter values
    typedef enum {
        Normal = 0,
        Grayscale,
        Sepia,
        Vivid,
        Negative,
        Solarize
    } ColorFilter;

    Pipeline(QObject *parent = 0);
    ~Pipeline();
    void setupFileStorage();
    void setVideoMode();
    void setResolution(const Resolution value);
    void setZoom(const double value);
    void setColorFilter(const ColorFilter value);
    void setVideoEffect(const QString &value);
    void start();
    void stop();
    void startRecording();
    void stopRecording();
    void setWindowId(int winId);
    void handleBusMessage(GstMessage *message);

 private:
    QString nextFileName();

    GstElement *camerabin;
    GstElement *viewfinder;
    GstElement *videoSrc;
    GstElement *camSrc;
};
#endif
