#include <QDeclarativeView>
#include <QApplication>
#include <QtDeclarative>

#include <gst/gst.h>

#include "controller.h"
#include "xvviewfinder.h"

#ifdef __i386__
#define QML_PATH "src/"
#else
#define QML_PATH "/usr/share/aura/"
#endif

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    QApplication app(argc, argv);

    QDeclarativeView view;
    qDebug("opening qml files in %s", QML_PATH);
    view.setSource(QUrl::fromLocalFile(QML_PATH "aura.qml"));
    view.showFullScreen();

    int ret = app.exec();

    gst_deinit();
    return ret;
}
