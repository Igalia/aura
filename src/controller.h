#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDeclarativeView>
#include <MApplicationWindow>
#include <QCamDevice>

class Controller : public QObject
{
    Q_OBJECT
 public:
    Controller(QObject *parent = 0);
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


 private slots:
    void startPipeline();
    void stopPipeline();
    void startRecording();
    void stopRecording();
    void displayClicked();
    void resourcesLost();

 private:
    MApplicationWindow mainWindow;
    QCamDevice device;

    // current config
    double currentZoom;
    Resolution currentResolution;
    ColorFilter currentColorFilter;
    QString currentVideoEffect;
};
#endif
