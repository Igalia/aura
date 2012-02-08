#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QtDeclarative>

#ifdef __i386__
#define QML_PATH "src/"
#else
#define QML_PATH "/usr/share/aura/"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;
    qDebug("opening qml files in %s", QML_PATH);
    view.setSource(QUrl::fromLocalFile(QML_PATH "aura.qml"));
    view.showFullScreen();
    return app.exec();
}
