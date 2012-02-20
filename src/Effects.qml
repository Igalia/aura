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
    opacity: 0
    visible: opacity > 0

    property real backgroundOpacity: 0
    property real objectsOpacity: 1

    Behavior on opacity
    {
        NumberAnimation { duration: 100 }
    }

    function show() {
        opacity = 1
    }

    function hide() {
        opacity = 0
    }

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "#080810"
        opacity: backgroundOpacity
    }

    ButtonColumn {
        id: effectColumn
        opacity: objectsOpacity
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: UIConstants.HEADER_DEFAULT_TOP_SPACING_LANDSCAPE
            bottomMargin: UIConstants.HEADER_DEFAULT_BOTTOM_SPACING_LANDSCAPE
        }

        Button {
            text: "Radioactiv"
        }
        Button {
            text: "Hulk"
        }
    }

    Button {
        id: acceptButton
        opacity: objectsOpacity
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
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
