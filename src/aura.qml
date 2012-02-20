import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants
import "file:///usr/lib/qt4/imports/com/nokia/extras/constants.js" as ExtrasConstants
import aura.viewfinder 1.0

Item {
    id: page
    width: screen.displayHeight; height: screen.displayWidth

    Component.onCompleted: {
        screen.allowedOrientations = Screen.Landscape
        console.debug("program loaded")
    }

    Item {
        id: mainPage
        anchors.fill: parent

        ViewFinder {
            id: viewFinder
            anchors.fill: parent

            Rectangle {
                id: viewFinderColorkeyPainter
                anchors.fill: parent
                color: "#080810"

                Component.onCompleted: console.debug("viewfinder colorkey painted")
            }
        }
        }

        Effects {
            id: effects
        }

        ToolIcon {
            id: shutter
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            iconId: "camera-shutter"
            enabled: !effects.visible
            onClicked: console.debug("shutter pressed")
        }

        ToolIcon {
            id: conf
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            iconId: "toolbar-view-menu"
            enabled: !effects.visible
            onClicked: effects.show()
        }
    }
}
