#ifndef SETTINGS_H
#define SETTINGS_H

#include "pipeline.h"

#define INTERNAL_EMMC_PATH        "/home/user/MyDocs"
#define APP_FOLDER                INTERNAL_EMMC_PATH"/aura"
#define INDEX_FILE                APP_FOLDER"/.index"

// zoom
#define ZOOM_MIN                  1.0
#define ZOOM_MAX                  10.0

// user configurable parameters
#define ZOOM_DEFAULT              ZOOM_MIN
#define VIDEO_RESOLUTION_DEFAULT  Pipeline::High
#define COLOR_FILTER_DEFAULT      Pipeline::Normal
#define VIDEO_EFFECT_DEFAULT      "Dice"

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
