TEMPLATE = app
TARGET = "aura"

QT += declarative
CONFIG += meegotouch link_pkgconfig
PKGCONFIG += gstreamer-video-0.10 glib-2.0
LIBS += -lqcamera
DEPENDPATH += . /usr/include/qcamera
INCLUDEPATH += . /usr/include/qcamera

# Input
HEADERS += settings.h       \
           controller.h     \
           xvviewfinder.h   \
           effect.h         \
           effectmanager.h  \
           debugutils.h     \
           cameffect.h      \
           qcamcapability_p.h \

SOURCES += main.cpp         \
           controller.cpp   \
           xvviewfinder.cpp \
           effect.cpp       \
           effectmanager.cpp \
           debugutils.c     \
           cameffect.cpp    \
#FORMS#

  unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr
  }
BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share

DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

#MAKE INSTALL

INSTALLS += target qmlgui desktop service iconxpm icon26 icon48 icon64

  target.path =$$BINDIR

  qmlgui.path = $$DATADIR/aura
  qmlgui.files += aura.qml

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
