#ifndef XV_VIEW_FINDER_H
#define XV_VIEW_FINDER_H

#include <QObject>
#include <MApplicationWindow>
#include <gst/gst.h>
#include <QCamAbstractViewFinder>
#include <QCamDevice>
#include <MWidget>

class XvViewFinder : public QCamAbstractViewfinder, public MWidget {
public:
    XvViewFinder(QCamDevice *device);
    virtual ~XvViewFinder();
    GstElement *makeSinkElement();
    bool hasWindowId();
    int getWindowId();

private:
    GstElement *m_sink;
    QCamDevice *m_device;
    MApplicationWindow *m_aw;
};

#endif /* VIEW_FINDER_H */
