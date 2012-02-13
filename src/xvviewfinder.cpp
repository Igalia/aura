#include <MApplication>
#include <gst/base/gstbasesink.h>
#include <gst/gstpad.h>
#include <gst/interfaces/xoverlay.h>
#include "xvviewfinder.h"

XvViewFinder::XvViewFinder(QCamDevice *device, QGraphicsItem *parent) :
    MWidget(parent),
    m_sink(0),
    m_device(device),
    m_aw(0)
{
    m_aw = MApplication::activeApplicationWindow();
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

    if (true == m_aw->isOnDisplay()) {
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(m_sink),
                                     getWindowId());
    }

    return m_sink;
}

bool XvViewFinder::hasWindowId()
{
    return true;
}

int XvViewFinder::getWindowId()
{
    // We need to use MApplicationWindow::viewport()->effectiveWinId() in order
    // to work with MeeGo and non-MeeGo graphics systems.
    // effectiveWinId() will look for a parent widget that has a native window
    // IF the viewport() is not native (Which is the case for MeeGo graphics system).
    return m_aw->viewport()->effectiveWinId();
}

void XvViewFinder::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit mousePressed();
}

void XvViewFinder::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    emit mouseReleased();
}
