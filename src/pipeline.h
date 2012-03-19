/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Víctor Jáquez <vjaquez@igalia.com>
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

#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>
#include <QSystemDeviceInfo>
#include "settings.h"
#include "controller-settings.h"
#include <gst/gst.h>
#include <gst/interfaces/photography.h>
#include <gst/interfaces/xoverlay.h>
#include <gst/video/video.h>

QTM_USE_NAMESPACE

class Pipeline : public QObject
{
    Q_OBJECT

 public:

    Q_PROPERTY(QString savedFileName
               READ savedFileName
               NOTIFY savedFileNameChanged)

    // resolution values
    typedef enum {
        Low = 0,
        Medium,
        High
    } Resolution;

    Pipeline(QObject *parent = 0);
    ~Pipeline();
    void setupFileStorage();
    void setVideoMode();
    void setResolution(const Resolution value);
    void setZoom(const double value);
    void setColorFilter(const ControllerSettings::ColorFilter value);
    void setVideoEffect(const QString &value);
    void start();
    void stop();
    void prepare();
    void startRecording();
    void stopRecording();
    void setWindowId(int winId);
    void handleBusMessage(GstMessage *message);
    bool isIdle();
    QString savedFileName() { return currentFile; };

 signals:
    void idleChanged(bool isIdle);
    void savedFileNameChanged(const QString &filename);
    void pipelinePlaying();

 private:
    QString nextFileName();
    void setupEffectBins();
    void writeMetadata();

    GstElement *camerabin;
    GstElement *videoSrc;
    GstElement *viewfinder;

    GstElement *effectBin;
    GstElement *effectValve;
    GstElement *effectInternalBin;
    GstElement *effect;
    GstElement *effectPreCS;
    GstElement *effectPostCS;
    GstElement *effectCapsFilter;

    int windowId;
    QSystemDeviceInfo systemInfo;
    QString currentFile;
};
#endif
