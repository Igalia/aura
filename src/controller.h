/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDeclarativeItem>
#include "pipeline.h"

class Controller : public QDeclarativeItem
{
    Q_OBJECT

 public:
    Q_PROPERTY(bool recording
               READ recording
               WRITE setRecording
               NOTIFY recordingChanged)
    Q_PROPERTY(QString videoEffect
               READ videoEffect
               WRITE setVideoEffect
               NOTIFY videoEffectChanged)
    Controller(QDeclarativeItem *parent = 0);
    ~Controller();
    void setupEffects();

    // user to set user defined values
    void setResolution(const Pipeline::Resolution value);
    void setZoom(const double value);
    void setColorFilter(const Pipeline::ColorFilter value);

    Pipeline::Resolution resolution() {return m_currentResolution;};
    double zoom() {return m_currentZoom;};
    Pipeline::ColorFilter colorFilter() {return m_currentColorFilter;};
    QString videoEffect() {return m_currentVideoEffect;};

    bool recording() { return m_recording; };

public slots:
    void setRecording(bool recording);
    void setVideoEffect(const QString &value);

signals:
    void recordingChanged(bool recording);
    void videoEffectChanged(const QString &effectName);

 public slots:
    void startPipeline();
    void stopPipeline();
    void startRecording();
    void stopRecording();
    void shutterClicked();

 private slots:
    void resourcesLost();
    void idleChanged(bool isIdle);

 private:
    Pipeline *m_pipeline;
    bool m_recording;
    // current config
    double m_currentZoom;
    Pipeline::Resolution m_currentResolution;
    Pipeline::ColorFilter m_currentColorFilter;
    QString m_currentVideoEffect;
};
#endif
