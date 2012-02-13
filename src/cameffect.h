/* -*- mode: c++ -*- */

#ifndef CAM_EFFECT_H
#define CAM_EFFECT_H

#include <qcamcapability.h>

#include "effect.h"

class CamEffect : public QCamCapability {
    Q_OBJECT
public:
    explicit CamEffect(QCamDevice *device, QObject *parent = 0);
    virtual ~CamEffect();
    Effect *value() const;
    bool setValue(Effect *value);
private:
    Q_DISABLE_COPY(CamEffect);
    Effect *m_effect;
};

#endif
