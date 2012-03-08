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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "pipeline.h"

#define INTERNAL_EMMC_PATH        "/home/user/MyDocs"
#define APP_FOLDER                INTERNAL_EMMC_PATH"/aura"
#define FILE_SUFFIX               "mp4"

// zoom
#define ZOOM_MIN                  1.0
#define ZOOM_MAX                  10.0

// user configurable parameters
#define ZOOM_DEFAULT              ZOOM_MIN
#define VIDEO_RESOLUTION_DEFAULT  Pipeline::Low
#define COLOR_FILTER_DEFAULT      Pipeline::Normal
#define VIDEO_EFFECT_DEFAULT      "None"

// pixel resolutions in video mode
#define LOW_RES_WIDTH             320
#define LOW_RES_HEIGHT            240
#define MID_RES_WIDTH             640
#define MID_RES_HEIGHT            480
#define HIGH_RES_WIDTH            848
#define HIGH_RES_HEIGHT           480
#define VIDEO_FRN                 30
#define VIDEO_FRD                 1

#endif
