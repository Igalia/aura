/* -*- mode: c++ -*- */

#ifndef QCAM_CAPABILITY_P_H
#define QCAM_CAPABILITY_P_H

#include <QString>
#include <QObject>

#include "qcamdevice.h"

class QCamCapabilityPrivate : public QObject {
Q_OBJECT
public:
    QCamCapabilityPrivate(QCamDevice *dev = 0);
    virtual ~QCamCapabilityPrivate();

    GstElement *pipeline() const;
    GstElement *videosource() const;

    QCamDevice *device;
    QString capaName;
    GstElement *notifySource;
    int notifyId;
    QCamCapability *q_ptr;
};


#endif  // QCAM_CAPABILITY_P_H

