#ifndef COMMON_H
#define COMMON_H

#ifdef __i386__
#define QML_PATH "src/"
#else
#define QML_PATH DATADIR "/" TARGET "/"
#endif

#define MAIN_QML_FILE QML_PATH TARGET ".qml"

#endif
