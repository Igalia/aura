QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
error("Use the qmake include with Qt4.4 or greater, on Debian that is qmake-qt4");
}

TEMPLATE = subdirs
SUBDIRS  = src

unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /opt/$${TARGET}
  }

  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  effects.path = $$DATADIR/gnome-video-effects
  effects.files = effects/*.effect

  icon64.files = data/icon64/aura.png
  icon64.path = $$DATADIR/icons/hicolor/64x64/apps

  splash.files = data/splash/aura-splash.jpg
  splash.path = $$DATADIR/aura

  bg.files = data/bg/aura-bg.png
  bg.path = $$DATADIR/aura

  INSTALLS += effects icon64 splash bg
}
