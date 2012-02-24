#include <gst/base/gstbasesink.h>
#include <gst/gstpad.h>
#include <gst/interfaces/xoverlay.h>
#include <QDeclarativeView>
#include <QDeclarativeEngine>

#include "xvviewfinder.h"
#include "common.h"

XvViewFinder::XvViewFinder(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_sink(0),
    m_device(0)
{
    setZValue(-1.0);

    qDebug() << "loading ViewFinder.qml";

    QDeclarativeEngine engine;
    QDeclarativeComponent component(&engine, QUrl::fromLocalFile(QML_PATH "ViewFinder.qml"));
    QDeclarativeItem *rect = dynamic_cast<QDeclarativeItem *>(component.create());
    rect->setParentItem(this);
}


XvViewFinder::~XvViewFinder()
{
    if (0 != m_sink) {
        gst_object_unref(m_sink);
        m_sink = 0;
    }
}

GstElement *XvViewFinder::makeSinkElement()
{
    if (0 != m_sink) {
        g_object_unref(m_sink);
        m_sink = 0;
    }

    QString sinkName = QString::fromAscii("omapxvsink");

    m_sink = gst_element_factory_make(sinkName.toAscii(), "xvsink");
    if (0 == m_sink) {
        return 0;
    }

    gst_object_ref(m_sink);

    g_object_set(m_sink, "autopaint-colorkey", FALSE, NULL);

    int winId = getWindowId();
    qDebug() << "setting win id to " << winId;
    gst_x_overlay_set_window_handle(GST_X_OVERLAY(m_sink), winId);

    return m_sink;
}

bool XvViewFinder::hasWindowId()
{
    return true;
}

int XvViewFinder::getWindowId()
{
    int winId = 0;

    QList<QGraphicsView*> viewsList = scene()->views();

    if (viewsList.size() == 1) {
        winId = viewsList[0]->effectiveWinId();
    } else {
        qCritical() << "there is " << viewsList.size() << "views";
    }

    return winId;
}
