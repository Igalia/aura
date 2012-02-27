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
    Pipeline m_pipeline;
    bool m_recording;
    // current config
    double m_currentZoom;
    Pipeline::Resolution m_currentResolution;
    Pipeline::ColorFilter m_currentColorFilter;
    QString m_currentVideoEffect;
};
#endif
