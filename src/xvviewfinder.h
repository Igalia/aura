#ifndef XV_VIEW_FINDER_H
#define XV_VIEW_FINDER_H

#include <QObject>
#include <gst/gst.h>
#include <QCamAbstractViewFinder>
#include <QCamDevice>
#include <QGraphicsSceneMouseEvent>
#include <QDeclarativeItem>

class XvViewFinder : public QDeclarativeItem, public QCamAbstractViewfinder {
    Q_OBJECT

public:
    XvViewFinder(QDeclarativeItem *parent = 0);
    virtual ~XvViewFinder();
    GstElement *makeSinkElement();
    bool hasWindowId();
    int getWindowId();
    void setCamDevice(QCamDevice *device) { m_device = device; };

private:
    GstElement *m_sink;
    QCamDevice *m_device;
};

#endif /* VIEW_FINDER_H */
