#include <QDebug>
#include <QDir>
#include "pipeline.h"
#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>

gboolean gStreamerMessageWatcher(GstBus *bus,
                                 GstMessage *message,
                                 gpointer data)
{
    Q_UNUSED(bus);

    static_cast<Pipeline*>(data)->handleBusMessage(message);

    // We always return true to keep the event source running (see
    // documentation for gst_bus_add_watch).
    return TRUE;
}

Pipeline::Pipeline(QObject *parent)
    : QObject(parent),
      camerabin(0)
{
    camerabin = gst_element_factory_make("camerabin2", NULL);
    g_object_set (camerabin, "post-previews", FALSE, NULL);
    g_object_set (camerabin, "flags", 0xf, NULL);

    // audio
    GstElement *audioSrc = gst_element_factory_make("pulsesrc", NULL);
    g_object_set(camerabin, "audio-source", audioSrc, NULL);
    GstCaps *audioCaps = gst_caps_from_string("audio/x-raw-int, channels=(int)2, width=(int)16, depth=(int)16, rate=(int)48000");
    g_object_set (camerabin,
                  "audio-capture-caps",
                  audioCaps,
                  NULL);


    // video
    videoSrc = gst_element_factory_make("subdevsrc", NULL);
    g_object_set(videoSrc, "queue-size", 5, NULL);
    camSrc = gst_element_factory_make("camsrcbin", NULL);
    g_object_set(camSrc, "video-source", videoSrc, NULL);
    g_object_set (G_OBJECT (camerabin), "camera-source", camSrc, NULL);

    // viewfinder
    viewfinder = gst_element_factory_make("omapxvsink", NULL);
    g_object_set(viewfinder, "autopaint-colorkey", FALSE, NULL);
    g_object_set (G_OBJECT (camerabin), "viewfinder-sink", viewfinder, NULL);

    // encoder profile
    // I should remove this and create a profile myself
    GstEncodingTarget *target =
        gst_encoding_target_load_from_file("/usr/share/libqcamera/libqcamera.gep", NULL);
    GstEncodingProfile *profile =
        gst_encoding_target_get_profile(target, "mpeg4-video");
    g_object_set(camerabin, "video-profile", profile, (void*) 0);

    //error handler
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(camerabin));
    gst_bus_add_watch(bus, gStreamerMessageWatcher, this);
    gst_object_unref(bus);

    // set initial values
    setupFileStorage();
    setVideoMode();
    setZoom(ZOOM_DEFAULT);
    setResolution(VIDEO_RESOLUTION_DEFAULT);
    setColorFilter(COLOR_FILTER_DEFAULT);
    setVideoEffect(VIDEO_EFFECT_DEFAULT);
}

Pipeline::~Pipeline()
{
    gst_object_unref(GST_OBJECT(camerabin));
}

void Pipeline::setVideoMode()
{
    g_object_set(camerabin, "mode", 1, NULL);

    // set auto scene mode
    gst_photography_set_scene_mode(GST_PHOTOGRAPHY(videoSrc),
                                   GST_PHOTOGRAPHY_SCENE_MODE_AUTO);
    // disable autofocus
    gst_photography_set_autofocus (GST_PHOTOGRAPHY(videoSrc), FALSE);
    // disable flash
    g_object_set(videoSrc, "video-torch", FALSE, NULL);
}

void Pipeline::setupFileStorage()
{
    //ensure that the storage directory exists, and creat it if needed
    if (!QDir::root().exists(APP_FOLDER)) {
        QDir::root().mkdir(APP_FOLDER);
    }

    // // set the file naming pattern
    // m_device.setFileNaming(new QCamSimpleFileNaming(APP_FOLDER, INDEX_FILE, &m_device));
}
void Pipeline::start()
{
    gst_element_set_state(camerabin, GST_STATE_PLAYING);
}

void Pipeline::stop()
{
    gst_element_set_state(camerabin, GST_STATE_NULL);
}

void Pipeline::startRecording()
{
    g_signal_emit_by_name(camerabin, "start-capture", 0);
}

void Pipeline::stopRecording()
{
    g_signal_emit_by_name(camerabin, "stop-capture", 0);
}

void Pipeline::setResolution(Resolution value)
{
    int width, height;
    int vfPosition, vfWidth;

    switch (value) {
    case Low:
        // QVGA
        width = LOW_RES_WIDTH;
        height = LOW_RES_HEIGHT;
        vfPosition = (848 - 640) / 2;
        vfWidth = 640;
        break;
    case Medium:
        // VGA
        width = MID_RES_WIDTH;
        height = MID_RES_HEIGHT;
        vfPosition = (848 - 640) / 2;
        vfWidth = 640;
        break;
    case High:
        // WVGA
        width = HIGH_RES_WIDTH;
        height = HIGH_RES_HEIGHT;
        vfPosition = 0;
        vfWidth = 848;
        break;
    default:
        qCritical() << "Unsupported resolution value " << value;
        return;
    }

    guint32 fcc = GST_MAKE_FOURCC('U', 'Y', 'V', 'Y');
    GstCaps *caps = 0;

    caps = gst_caps_new_full(gst_structure_new("video/x-raw-yuv",
                                               "format", GST_TYPE_FOURCC, fcc,
                                               "width", G_TYPE_INT, width,
                                               "height", G_TYPE_INT, height,
                                               "framerate",
                                                GST_TYPE_FRACTION_RANGE,
                                                VIDEO_FRN - 1, VIDEO_FRD,
                                                VIDEO_FRN + 1, VIDEO_FRD,
                                                (void*) 0),
                              (void*) 0);

    // set caps to the viewfinder
    g_object_set(camerabin, "viewfinder-caps", caps, NULL);
    g_object_set(camerabin, "video-capture-caps", caps, NULL);
    gst_caps_unref(caps);

    // set new rendering position to the viewfinder
    gst_x_overlay_set_render_rectangle(GST_X_OVERLAY(viewfinder),
                                       vfPosition,
                                       0,
                                       vfWidth,
                                       480);
}

void Pipeline::setZoom(double value)
{
    g_object_set (camerabin, "zoom", value, NULL);
}

void Pipeline::setColorFilter(ColorFilter value)
{
    GstColourToneMode mode;

    switch (value) {
    case Normal:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_NORMAL;
        break;
    case Grayscale:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_GRAYSCALE;
        break;
    case Sepia:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_SEPIA;
        break;
    case Vivid:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_VIVID;
        break;
    case Negative:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_NEGATIVE;
        break;
    case Solarize:
        mode = GST_PHOTOGRAPHY_COLOUR_TONE_MODE_SOLARIZE;
        break;
    default:
        qCritical() << "Unsupported color filter  value " << value;
        return;
    }

    g_object_set(videoSrc, "colour-tone-mode", mode, NULL);
}

void Pipeline::setVideoEffect(const QString &value)
{
    // CamEffect *cameff = new CamEffect(&m_device, this);
    // if (cameff->setValue(EffectManager::instance()->getEffect(value))) {
    //     m_currentVideoEffect = value;
    // } else {
    //     qCritical() << "Error setting video effect " << value;
    // }
}

void Pipeline::setWindowId(int winId)
{
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(viewfinder), winId);
}

void Pipeline::handleBusMessage(GstMessage *message)
{
    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ELEMENT:
        {
            break;
        }
    case GST_MESSAGE_ERROR:
        {
            GError *gerror = 0;
            gchar *debug = 0;
            gst_message_parse_error(message, &gerror, &debug);
            qCritical() << "Debug" << debug << " Error " << gerror->message;
            g_free(debug);
            g_error_free(gerror);
            break;
        }

    case GST_MESSAGE_WARNING:
        {
            GError *gerror = 0;
            gchar *debug = 0;
            gst_message_parse_warning(message, &gerror, &debug);
            qCritical() << "Debug" << debug << " Warning " << gerror->message;
            g_free(debug);
            g_error_free(gerror);
            break;
        }

    case GST_MESSAGE_INFO:
        {
            GError *gerror = 0;
            gchar *debug = 0;
            gst_message_parse_info(message, &gerror, &debug);
            qCritical() << "Debug" << debug << " Info " << gerror->message;
            g_free(debug);
            g_error_free(gerror);
            break;
        }

    case GST_MESSAGE_STATE_CHANGED:
        {
            if (GST_ELEMENT(GST_MESSAGE_SRC(message)) == camerabin) {
                GstState oldstate, newstate, pending;
                gst_message_parse_state_changed(message, &oldstate, &newstate, &pending);
                if (newstate==GST_STATE_PLAYING) {
                    qCritical() << "pipeline started";
                }
                if (newstate==GST_STATE_READY && oldstate==GST_STATE_PAUSED) {
                    qCritical() << "pipeline paused";
                }
            } else if (strcmp(GST_OBJECT_NAME (GST_MESSAGE_SRC(message)), "video-encodebin") == 0) {
                GstState oldstate, newstate, pending;

                gst_message_parse_state_changed(message, &oldstate, &newstate, &pending);
                if (newstate==GST_STATE_PLAYING) {
                    qCritical() << "Video Elements ready";
                }
            }
            break;
        }

    default:
        break;
    }
}
