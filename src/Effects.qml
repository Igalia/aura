import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants
import "file:///usr/lib/qt4/imports/com/nokia/extras/constants.js" as ExtrasConstants

// http://developer.qt.nokia.com/wiki/Modern_mobile_applications_with_Qt_and_QML

Rectangle {
    id: effectsPage
    anchors.fill: parent
    color: "transparent"
    visible: false

    property int animationDuration: 150
    property string selectedEffectName: ""
    property string initialEffect: ""

    property variant __effectNames: effectManager.effectNames();

    states: [
        State {
            name: "uninitialized"
            when: initialEffect == ""
        },
        State {
            name: "initialized"
            when: initialEffect != ""
            StateChangeScript {
                name: "settingButtonColumn"
                script: {
                    console.debug("setting initial effect to " + initialEffect)
                    effectColumn.checkedButton = repeater.itemAt(__effectNames.indexOf(initialEffect))
                }
            }
            PropertyChanges { target: effectsPage; selectedEffectName: effectColumn.checkedButton.effectName }
        }
    ]

    function show() {
        visible = true
    }

    function hide() {
        visible = false
    }

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "#080810"
    }

    Flickable {
        id: flickable
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: acceptButtonItem.top
            topMargin: UIConstants.HEADER_DEFAULT_TOP_SPACING_LANDSCAPE
            bottomMargin: UIConstants.HEADER_DEFAULT_BOTTOM_SPACING_LANDSCAPE
            leftMargin: UIConstants.DEFAULT_MARGIN
            rightMargin: UIConstants.DEFAULT_MARGIN
        }
        contentHeight: flickableButtonBorder.height
        clip: true

        ButtonBorder {
            id: flickableButtonBorder
            anchors.fill: effectColumn
        }

        ButtonColumn {
            id: effectColumn
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            Repeater {
                id: repeater
                model: __effectNames.length

                Button {
                    property string effectName: __effectNames[index]
                    text: effectName
                }
            }
        }
    }

    Item {
        id: acceptButtonItem
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            topMargin: UIConstants.HEADER_DEFAULT_TOP_SPACING_LANDSCAPE
            bottomMargin: UIConstants.HEADER_DEFAULT_BOTTOM_SPACING_LANDSCAPE
        }
        width: acceptButtonBorder.width
        height: acceptButtonBorder.height

        ButtonBorder {
            id: acceptButtonBorder
            anchors.fill: acceptButton
        }

        Button {
            id: acceptButton
            text: "Accept"
            font.family: UIConstants.FONT_FAMILY
            onClicked: {
                console.debug("accept")
                effectsPage.hide()
            }
        }
    }
}
