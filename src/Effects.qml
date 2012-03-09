/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

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
    property int selectedEffectIndex
    property string initialEffect: ""

    property variant effectNames: []

    states: [
        State {
            name: "uninitialized"
            when: initialEffect == "" || effectNames.length == 0
        },
        State {
            name: "initialized"
            when: initialEffect != "" && effectNames.length > 0
            StateChangeScript {
                name: "settingButtonColumn"
                script: {
                    console.debug("setting initial effect to " + initialEffect)
                    effectColumn.checkedButton = repeater.itemAt(effectNames.indexOf(initialEffect))
                }
            }
            PropertyChanges { target: effectsPage; selectedEffectName: effectColumn.checkedButton.effectName }
            PropertyChanges { target: effectsPage; selectedEffectIndex: effectColumn.checkedButton.effectIndex }
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
                model: effectNames.length

                Button {
                    property int effectIndex: index
                    property string effectName: effectNames[index]
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
