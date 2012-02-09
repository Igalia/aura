import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants
import "file:///usr/lib/qt4/imports/com/nokia/extras/constants.js" as ExtrasConstants

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    Component.onCompleted: {
        console.debug("program loaded")
    }

    Page {
        id: mainPage
        orientationLock: PageOrientation.LockLandscape

        Text {
            id: hello
            anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
            width: paintedWidth
            height: paintedHeight
            font.pixelSize: UIConstants.FONT_XLARGE
            font.family: UIConstants.FONT_FAMILY
            text: "Hello world"
            clip: true
        }
    }
}
