import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants
import "file:///usr/lib/qt4/imports/com/nokia/extras/constants.js" as ExtrasConstants
import aura.controller 1.0

Item {
    id: page
    width: screen.displayWidth; height: screen.displayHeight

    Component.onCompleted: {
        screen.allowedOrientations = Screen.Landscape
        console.debug("program loaded")
    }

    property bool windowActive : platformWindow.active
    property int animationDuration: 150

    Item {
        id: mainPage
        anchors.fill: parent

        Connections {
            target: platformWindow

            onActiveChanged: {
                if (platformWindow.active) {
                    controller.startPipeline()
                } else {
                    controller.stopPipeline()
                }
            }
        }

        Rectangle {
            id: viewFinderColorkeyPainter
            anchors.fill: parent
            color: "#080810"

            Component.onCompleted: console.debug("viewfinder colorkey painted")
        }

        Controller {
            id: controller
            onRecordingChanged: console.debug("recording changed to " + recording)
            onVideoEffectChanged: console.debug("video effect changed to " + effectName)

            property bool __completed: false

            Component.onCompleted: {
                console.debug("controller loaded, setting " + videoEffect + " as default to the effects page")
                effects.initialEffect = videoEffect
                __completed = true
            }
        }

        Effects {
            id: effects
            animationDuration: page.animationDuration
        }

        Binding {
            target: controller
            property: "videoEffect"
            value: effects.selectedEffectName
            when: controller.__completed
        }

        ToolIcon {
            id: shutter
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            iconId: "camera-shutter"
            opacity: effects.visible ? 0 : 1
            visible: opacity > 0
            Behavior on opacity { NumberAnimation { duration: animationDuration } }
            onClicked: {
                console.debug("shutter clicked")
                controller.shutterClicked()
            }
        }

        ToolIcon {
            id: conf
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            iconId: "toolbar-view-menu"
            opacity: effects.visible || controller.recording ? 0 : 1
            visible: opacity > 0
            Behavior on opacity { NumberAnimation { duration: animationDuration } }
            onClicked: effects.show()
        }

        Item {
            anchors.fill: parent
            visible: !platformWindow.active

            Rectangle {
                anchors.fill: parent
                color: "grey"
            }

            Text {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                font.pixelSize: UIConstants.FONT_XLARGE
                font.family: UIConstants.FONT_FAMILY
                text: "Camera is paused"
            }
        }
    }
}
