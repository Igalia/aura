#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDeclarativeItem>
#include "pipeline.h"

class Controller : public QDeclarativeItem
{
    Q_OBJECT

 public:
    Controller(QDeclarativeItem *parent = 0);
    void setupEffects();

    // user to set user defined values
    void setResolution(const Pipeline::Resolution value);
    void setZoom(const double value);
    void setColorFilter(const Pipeline::ColorFilter value);
    void setVideoEffect(const QString &value);

 public slots:
    void setup();
    void startPipeline();
    void stopPipeline();
    void startRecording();
    void stopRecording();
    void shutterClicked();

 private slots:
    void resourcesLost();

 private:
    Pipeline pipeline;
    bool recording;
    // current config
    double currentZoom;
    Pipeline::Resolution currentResolution;
    Pipeline::ColorFilter currentColorFilter;
    QString currentVideoEffect;
};
#endif
