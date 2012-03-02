#include <QDebug>
#include <QDir>
#include <QDate>
#include <QStringList>

#include "pipeline.h"
#include "effectmanager.h"
#include "effect.h"

#include <gst/pbutils/encoding-profile.h>
#include <gst/pbutils/encoding-target.h>
#include <gst/basecamerabinsrc/gstcamerabin-enum.h>


gboolean gStreamerMessageWatcher(GstBus *bus,
                                 GstMessage *message,
                                 gpointer data)
{
    Q_UNUSED(bus);

    static_cast<Pipeline*>(data)->handleBusMessage(message);

    return TRUE;
}

static void
idleNotificationCallback(GObject *,
                         GParamSpec *,
                         gpointer user_data)
{
    Pipeline *pipeline = static_cast<Pipeline*>(user_data);

    // This signal gets sent with queued connection always to avoid
    // running stuff within the scope of the property notifications.
    QMetaObject::invokeMethod(pipeline,
                              "idleChanged",
                              Qt::QueuedConnection,
                              Q_ARG(bool, pipeline->isIdle()));
}

Pipeline::Pipeline(QObject *parent)
    : QObject(parent),
      camerabin(0),
      videoSrc(0),
      viewfinder(0),
      effectBin(0),
      effectPreValve(0),
      effect(0),
      effectPreCS(0),
      effectPostCS(0),
      effectCapsFilter(0)
{
    // camerabin
    camerabin = gst_element_factory_make("camerabin", NULL);
    g_object_set (camerabin, "framerate-rounding", TRUE, NULL);

    // video source
    videoSrc = gst_element_factory_make("subdevsrc", NULL);
    g_object_set(videoSrc, "queue-size", 5, NULL);
    g_object_set (G_OBJECT (camerabin), "video-source", videoSrc, NULL);

    // video encoder
    GstElement *videoEnc = gst_element_factory_make("dsph264enc", NULL);
    g_object_set(camerabin, "video-encoder", videoEnc, NULL);

    // videomux
    GstElement *videoMux = gst_element_factory_make("mp4mux", NULL);
    g_object_set(camerabin, "video-muxer", videoMux, NULL);

    // audio source
    GstElement *audioSrc = gst_element_factory_make("pulsesrc", NULL);
    g_object_set(camerabin, "audio-source", audioSrc, NULL);

    // audio encoder
    // we need to set a capsfilter in order to select the audio config
    GstElement *audioEncBin = gst_bin_new(NULL);
    GstElement *audioEnc = gst_element_factory_make("nokiaaacenc", NULL);
    GstElement *audioCapsFilter = gst_element_factory_make("capsfilter", NULL);
    GstCaps *audioCaps = gst_caps_from_string("audio/x-raw-int, channels=(int)2, width=(int)16, depth=(int)16, rate=(int)48000");
    g_object_set(audioCapsFilter, "caps", audioCaps, NULL);

    gst_bin_add_many(GST_BIN(audioEncBin), audioCapsFilter, audioEnc, NULL);
    gst_element_link(audioCapsFilter, audioEnc);

    GstPad * pad;
    pad = gst_element_get_static_pad (audioCapsFilter, "sink");
    gst_element_add_pad (audioEncBin, gst_ghost_pad_new ("sink", pad));
    gst_object_unref (pad);

    pad = gst_element_get_static_pad (audioEnc, "src");
    gst_element_add_pad (audioEncBin, gst_ghost_pad_new ("src", pad));
    gst_object_unref (pad);

    g_object_set(camerabin, "audio-encoder", audioEncBin, NULL);

    // viewfinder
    viewfinder = gst_element_factory_make("omapxvsink", NULL);
    g_object_set(viewfinder, "autopaint-colorkey", FALSE, NULL);
    g_object_set (G_OBJECT (camerabin), "viewfinder-sink", viewfinder, NULL);

    //error handler
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(camerabin));
    gst_bus_add_watch(bus, gStreamerMessageWatcher, this);
    gst_object_unref(bus);

    // idle handler
    g_signal_connect(G_OBJECT(camerabin),
                     "notify::idle",
                     G_CALLBACK(idleNotificationCallback),
                     this);

    // set initial values
    setVideoMode();
    setupFileStorage();
    setupEffectBins();
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
}

void Pipeline::start()
{
    gst_element_set_state(camerabin, GST_STATE_PLAYING);
}

void Pipeline::stop()
{
    gst_element_set_state(camerabin, GST_STATE_PAUSED);
}

void Pipeline::startRecording()
{
    // set next file name
    g_object_set(camerabin,
                 "filename",
                 nextFileName().toUtf8().constData(),
                 NULL);

    g_signal_emit_by_name(camerabin, "capture-start", 0);
}

void Pipeline::stopRecording()
{
    g_signal_emit_by_name(camerabin, "capture-stop", 0);
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

    g_signal_emit_by_name(camerabin, "set-video-resolution-fps",
                          width, height, VIDEO_FRN, VIDEO_FRD, NULL);

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
    Effect *newEffect = EffectManager::instance()->getEffect(value);
    qCritical() << "Pipeline: setting effect " << newEffect->name() << " with pipeline desc : " << newEffect->desc();

    // close valves
    g_object_set(effectPreValve, "drop", TRUE, NULL);

    // unlink current effect, remove and destroy it
    gst_element_unlink_many(effectCapsFilter, effect, effectPostCS, NULL);
    g_object_ref(effect);
    gst_bin_remove(GST_BIN(effectBin), effect);
    gst_element_set_state(effect, GST_STATE_NULL);
    g_object_unref(GST_OBJECT(effect));

    effect = gst_parse_bin_from_description(newEffect->desc().toUtf8(), TRUE, NULL);

    // add new effect to the bin and link it
    gst_bin_add(GST_BIN(effectBin), effect);
    gst_element_link_many(effectCapsFilter, effect, effectPostCS, NULL);

    gst_element_set_state(effect, GST_STATE_PAUSED);

    //open valve
    g_object_set(effectPreValve, "drop", FALSE, NULL);
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

bool Pipeline::isIdle()
{
    gboolean result = FALSE;
    g_object_get(camerabin, "idle", &result, NULL);
    return (TRUE == result);
}

QString Pipeline::nextFileName()
{
    QString date = QDate::currentDate().toString("yyMMdd");

    QDir dir(APP_FOLDER);
    QStringList filters(QString("*%1*").arg(date));
    dir.setNameFilters(filters);
    int index = dir.count() + 1;

    QString filename(APP_FOLDER);
    filename += QDir::separator() + QString("%1_%2.%3").arg(date).arg(index).arg(FILE_SUFFIX);
    return filename;
}

void Pipeline::setupEffectBins()
{
    effectBin = gst_bin_new(NULL);
    effectPreValve = gst_element_factory_make("valve", NULL);
    effectPreCS = gst_element_factory_make("ffmpegcolorspace", NULL);
    effectPostCS = gst_element_factory_make("ffmpegcolorspace", NULL);

    // I need to add this capsfilter because when setting an identity filter,
    // there's some problem negotiating the capabilites and the viewfinder doesn't work
    effectCapsFilter = gst_element_factory_make("capsfilter", NULL);
    GstCaps *caps = gst_caps_from_string("video/x-raw-rgb");
    g_object_set(effectCapsFilter, "caps", caps, NULL);

    // This element will contain the effect bin. At this point we set
    // an identity element so no effect is applied
    effect = gst_element_factory_make("identity", NULL);

    gst_bin_add_many(GST_BIN(effectBin), effectPreValve, effectPreCS, effectCapsFilter, effect, effectPostCS, NULL);
    gst_element_link_many(effectPreValve, effectPreCS, effectCapsFilter, effect, effectPostCS, NULL);

    GstPad *pad;
    pad = gst_element_get_static_pad(effectPreValve, "sink");
    gst_element_add_pad(effectBin, gst_ghost_pad_new("sink", pad));
    gst_object_unref(GST_OBJECT(pad));

    pad = gst_element_get_static_pad(effectPostCS, "src");
    gst_element_add_pad(effectBin, gst_ghost_pad_new("src", pad));
    gst_object_unref(GST_OBJECT(pad));

    g_object_set(camerabin, "video-source-filter", effectBin, NULL);
}
