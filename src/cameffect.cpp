/* -*- mode: c++ -*- */

#include <QDebug>

#include <gst/gst.h>

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

    GstElement *item = 0;
    GstIterator *iter = gst_bin_iterate_elements(GST_BIN(vffbin));
    bool done = false;
    while (!done) {
	    switch(gst_iterator_next(iter, (void **) &item)) {
	    case GST_ITERATOR_OK:
		    qDebug() << "+ element in bin name"
			     << gst_element_get_name(item);
		    gst_object_unref(item);
		    break;
	    default:
		    done = true;
		    break;
	    }
    }
    gst_iterator_free(iter);

    return true;
}
