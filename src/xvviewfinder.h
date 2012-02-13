#ifndef XV_VIEW_FINDER_H
#define XV_VIEW_FINDER_H

#include <QObject>
#include <MApplicationWindow>
#include <gst/gst.h>
#include <QCamAbstractViewFinder>
#include <QCamDevice>
#include <MWidget>
#include <QGraphicsSceneMouseEvent>

class XvViewFinder : public MWidget, public QCamAbstractViewfinder {
    Q_OBJECT
public:
    XvViewFinder(QCamDevice *device, QGraphicsItem *parent = 0);
    virtual ~XvViewFinder();
    GstElement *makeSinkElement();
    bool hasWindowId();
    int getWindowId();

signals:
    void mousePressed();
    void mouseReleased();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:
    GstElement *m_sink;
    QCamDevice *m_device;
    MApplicationWindow *m_aw;
};

#endif /* VIEW_FINDER_H */
