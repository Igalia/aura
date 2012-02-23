#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>
#include "settings.h"

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

 private:
    
};
#endif
