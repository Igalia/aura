#include <MApplication>

#include <gst/gst.h>

#include "controller.h"

#ifdef __i386__
#define QML_PATH "src/"
#else
#define QML_PATH "/usr/share/aura/"
#endif

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    MApplication *app = new MApplication(argc, argv);

    Controller *c = new Controller();

    int ret = app->exec();

    delete c;
    delete app;
    gst_deinit();
    return ret;
}
