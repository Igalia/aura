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

        Rectangle {
            id: rectangle
            anchors.fill: parent
            color: "red"
        }

        ToolIcon {
            id: shutter
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            iconId: "camera-shutter"
            onClicked: console.debug("shutter pressed")
        }

        ToolIcon {
            id: conf
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            iconId: "toolbar-view-menu"
            onClicked: console.debug("conf pressed")
        }
    }
}
