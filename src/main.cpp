#include <QDeclarativeView>
#include <QApplication>
#include <QtDeclarative>

#include <gst/gst.h>

#include "controller.h"

#include "common.h"

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    QApplication app(argc, argv);

    QDeclarativeView view;
    qmlRegisterType<Controller>("aura.controller", 1, 0, "Controller");
    qDebug("opening qml files in %s", QML_PATH);
    view.setSource(QUrl::fromLocalFile(MAIN_QML_FILE));
    view.showFullScreen();

    int ret = app.exec();

    gst_deinit();
    return ret;
}
