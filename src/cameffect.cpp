/* -*- mode: c++ -*- */

#include <QDebug>

#include <gst/gst.h>

#include "debugutils.h"

#include "cameffect.h"
#include "qcamcapability_p.h"

QCamCapabilityPrivate::QCamCapabilityPrivate(QCamDevice *dev) :
    device(dev),
    notifySource(0),
    notifyId(-1),
    q_ptr(0)
{
}

QCamCapabilityPrivate::~QCamCapabilityPrivate()
{
    device = 0;
    notifySource = 0;
    notifyId = -1;
}

GstElement *
QCamCapabilityPrivate::pipeline() const
{
    if (0 == device) {
        return 0;
    }

    return device->pipeline();
}


GstElement *
QCamCapabilityPrivate::videosource() const
{
    if (0 == device) {
        return 0;
    }

    return device->videoSrc();
}


CamEffect::CamEffect(QCamDevice *device,
		     QObject *parent) :
    QCamCapability(new QCamCapabilityPrivate(device), parent),
    m_effect(0)
{
}

CamEffect::~CamEffect()
{
	m_effect = 0;
}

Effect *
CamEffect::value() const
{
    return m_effect;
}

bool
CamEffect::setValue(Effect *value)
{
    Q_D(QCamCapability);

    m_effect = value;

    GstElement *camerabin = d->pipeline();
    GstElement *vffbin = 0;

    g_object_get(camerabin, "viewfinder-filter", &vffbin, NULL);
    if (0 == vffbin) {
	    return false;
    }

    qDebug() << "vffbin element name"
	     << gst_element_get_name(vffbin);

    my_debug_bin_to_dot_file(GST_BIN(vffbin),
			     GST_DEBUG_GRAPH_SHOW_ALL,
			     "vffbin");

    return true;
}
