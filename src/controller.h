#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDeclarativeItem>
#include <QCamDevice>

#include "xvviewfinder.h"

class Controller : public QDeclarativeItem
{
    Q_OBJECT

 public:
    Q_PROPERTY(QObject * viewFinder
               READ viewFinder
               WRITE setViewFinder)
    Controller(QDeclarativeItem *parent = 0);
    void setupCamDevice();
    void setVideoMode();
    void setupViewfinder();
    void setupEffects();
    void setupFileStorage();

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

    // user to set user defined values
    void setResolution(const Resolution value);
    void setZoom(const double value);
    void setColorFilter(const ColorFilter value);
    void setVideoEffect(const QString &value);


    QObject* viewFinder() { return m_viewFinder; };

 public slots:
    void setViewFinder(QObject *viewFinder) {
      m_viewFinder = dynamic_cast<XvViewFinder*>(viewFinder);
    };

    void setup();
    void startPipeline();
    void stopPipeline();
    void startRecording();
    void stopRecording();
    void shutterClicked();

 private slots:
    void resourcesLost();

 private:
    QCamDevice m_device;
    XvViewFinder *m_viewFinder;
    // current config
    double m_currentZoom;
    Resolution m_currentResolution;
    ColorFilter m_currentColorFilter;
    QString m_currentVideoEffect;
};
#endif
