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
    void setupViewfinder();

 private slots:
    void startPipeline();
    void stopPipeline();

 private:
    MApplicationWindow mainWindow;
    QCamDevice device;
};
#endif
