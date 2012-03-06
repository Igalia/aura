#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QDeclarativeItem>

class ControllerSettings : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(ColorFilter)

 public:
    // colour filter values
    enum ColorFilter {
        Normal = 0,
        Grayscale,
        Sepia,
        Vivid,
        Negative,
        Solarize
    };
};

#endif
