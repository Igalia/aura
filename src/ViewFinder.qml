import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Rectangle {
    id: viewFinderColorkeyPainter
    anchors.fill: parent
    color: "#080810"

    Component.onCompleted: console.debug("viewfinder colorkey painted")
}
