TEMPLATE = app
TARGET = "aura"

QT += declarative
CONFIG += meegotouch link_pkgconfig mobility
PKGCONFIG += gstreamer-0.10 glib-2.0 libresourceqt1 gstreamer-plugins-bad-0.10 \
             gstreamer-pbutils-0.10 gstreamer-interfaces-0.10
MOBILITY += systeminfo

QMAKE_CXXFLAGS_RELEASE += -Werror
QMAKE_CXXFLAGS_DEBUG += -g -O0 -Werror

# Input
HEADERS += settings.h       \
           controller.h     \
           effect.h         \
           effectmanager.h  \
           resourcemanager.h \
           pipeline.h       \

SOURCES += controller.cpp   \
           effect.cpp       \
           effectmanager.cpp \
           resourcemanager.cpp \
           pipeline.cpp     \
           main.cpp         \
#FORMS#

  unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr
  }
BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share

DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\" \
           TARGET=\\\"$$TARGET\\\" GST_USE_UNSTABLE_API

#MAKE INSTALL

INSTALLS += target qmlgui desktop service iconxpm icon26 icon48 icon64

  target.path =$$BINDIR

  qmlgui.path = $$DATADIR/aura
  qmlgui.files += aura.qml Effects.qml ButtonBorder.qml

  desktop.path = $$DATADIR/applications
  desktop.files += $${TARGET}.desktop

  service.path = $$DATADIR/dbus-1/services/
  service.files += com.meego.$${TARGET}.service

  iconxpm.path = $$DATADIR/pixmap
  iconxpm.files += ../data/maemo/$${TARGET}.xpm

  icon26.path = $$DATADIR/icons/hicolor/26x26/apps
  icon26.files += ../data/26x26/$${TARGET}.png

  icon48.path = $$DATADIR/icons/hicolor/48x48/apps
  icon48.files += ../data/48x48/$${TARGET}.png

  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
  icon64.files += ../data/64x64/$${TARGET}.png
}
