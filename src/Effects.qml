/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Víctor Jáquez <vjaquez@igalia.com>
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
    anchors {
        top: parent.top
        bottom: parent.bottom
        right: parent.right
    }
    width: 228
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

    Flickable {
        id: flickable
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: acceptButton.top
            topMargin: 30
            bottomMargin: 30
            rightMargin: 30
        }
        contentHeight: effectColumn.height
        clip: true

        ButtonColumn {
            id: effectColumn
            anchors {
                right: parent.right
            }
            width: parent.width
            Repeater {
                id: repeater
                model: effectNames.length

                Button {
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/button-bg.png"
                        checkedBackground: "qrc:/resources/button-bg-selected.png"
                        pressedBackground: "qrc:/resources/button-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    width: 198
                    height: 50
                    property int effectIndex: index
                    property string effectName: effectNames[index]
                    text: effectName
                }
            }
        }
    }

    Button {
        id: acceptButton
        anchors {
            right: parent.right
            bottom: parent.bottom
            left: parent.left
            rightMargin: 30
            bottomMargin: 30
        }
        platformStyle: ButtonStyle {
            background: "qrc:/resources/button-bg.png"
            checkedBackground: "qrc:/resources/button-bg-selected.png"
            pressedBackground: "qrc:/resources/button-bg-pressed.png"
            textColor: "white"
            fontPixelSize: UIConstants.FONT_DEFAULT
            fontFamily: UIConstants.FONT_FAMILY
        }
        width: 198
        height: 50
        text: "Accept"
        onClicked: {
            effectsPage.hide()
        }
    }
}
