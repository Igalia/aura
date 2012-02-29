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

    signal effectChanged(string effectName)

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
            bottom: acceptButton.top
            topMargin: UIConstants.HEADER_DEFAULT_TOP_SPACING_LANDSCAPE
            bottomMargin: UIConstants.HEADER_DEFAULT_BOTTOM_SPACING_LANDSCAPE
            leftMargin: UIConstants.DEFAULT_MARGIN
            rightMargin: UIConstants.DEFAULT_MARGIN
        }
        contentHeight: effectColumn.height
        clip: true

        ButtonColumn {
            id: effectColumn
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            onCheckedButtonChanged: effectChanged(checkedButton.effectName)

            Repeater {
                model: effectManager.effectNames()
                Button {
                    property string effectName: modelData
                    text: effectName
                }
            }
        }
    }

    Button {
        id: acceptButton
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            topMargin: UIConstants.HEADER_DEFAULT_TOP_SPACING_LANDSCAPE
            bottomMargin: UIConstants.HEADER_DEFAULT_BOTTOM_SPACING_LANDSCAPE
        }
        text: "Accept"
        font.family: UIConstants.FONT_FAMILY
        onClicked: {
            console.debug("accept")
            effectsPage.hide()
        }
    }
}
