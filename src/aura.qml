/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Víctor Jáquez <vjaquez@igalia.com>
 *          Michele Tameni <michele@tameni.it>
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
import QtMobility.systeminfo 1.2
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants
import "file:///usr/lib/qt4/imports/com/nokia/extras/constants.js" as
    ExtrasConstants
import aura.controller 1.0
import aura.postcapture 1.0

PageStackWindow {
    id: appWindow

    initialPage: page
    showStatusBar: false

    Page {
        id: page
        orientationLock: PageOrientation.LockLandscape
        width: screen.displayWidth; height: screen.displayHeight

        Component.onCompleted: {
            screen.allowedOrientations = Screen.Landscape
            console.debug("program loaded")
        }

        property bool windowActive : platformWindow.active
        property bool __dialogsVisible: effects.visible || colorFilters.visible
        property real __dimmedOpacity: 0.3
        property int __animationDuration: 50

        Item {
            id: mainPage
            anchors.fill: parent

            Connections {
                target: platformWindow

                onActiveChanged: {
                    if (!platformWindow.active) {
                        controller.stopPipeline()
                    } else if (!aboutView.visible) {
                        controller.startPipeline()
                    }
                }
            }

            Rectangle {
                id: ui
                anchors.fill: parent

                Image {
                    anchors.fill: parent
                    source: "qrc:/resources/aura-bg.png"
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        horizontalCenterOffset: -114
                        verticalCenter: parent.verticalCenter
                    }
                    visible: !platformWindow.active
                    font.pixelSize: UIConstants.FONT_XLARGE
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Paused..."
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        horizontalCenterOffset: -114
                        verticalCenter: parent.verticalCenter
                    }
                    visible: platformWindow.active && controller.pipelineStarting
                    font.pixelSize: UIConstants.FONT_XLARGE
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Loading..."
                }

                Button {
                    id: about
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/button-bg.png"
                        disabledBackground: "qrc:/resources/button-bg.png"
                        pressedBackground: "qrc:/resources/button-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    anchors {
                        top: parent.top
                        topMargin: 30
                        right: parent.right
                        rightMargin: 30
                    }
                    height: 50
                    width: 198
                    text: "About"
                    enabled: controller.pipelineReady || aboutView.visible
                    visible: !controller.recording && !page.__dialogsVisible
                    onClicked: aboutView.visible = !aboutView.visible
                }

                AboutView {
                    id: aboutView
                    visible: false
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        horizontalCenterOffset: -115
                        verticalCenter: parent.verticalCenter
                    }
                    width: 560
                    height: 420
                    clip: true

                    appString: "Aura"
                    iconSource: "file:///opt/aura/share/icons/hicolor/" +
                        "64x64/apps/aura.png"
                    email: "ovi-store@igalia.com"
                    twitterUrl: "https://twitter.com/#!/Igalia"
                    copyright: "Copyright © 2012 Igalia"
                    storeUrl: "http://store.ovi.com/content/266783"
                    websiteUrl: "http://www.igalia.com"
                    license:
                        "This program is free software; you can redistribute " +
                        "it and/or modify it under the terms of the GNU " +
                        "Lesser General Public License as published by the " +
                        "Free Software Foundation; version 2.1 of the " +
                        "License, or (at your option) any later version." +
                        "<br/><br/>" +

                        "This program is distributed in the hope that it " +
                        "will be useful, but WITHOUT ANY WARRANTY; without " +
                        "even the implied warranty of MERCHANTABILITY or " +
                        "FITNESS FOR A PARTICULAR PURPOSE. See the GNU " +
                        "Lesser General Public License for more details." +
                        "<br/><br/>" +

                        "You should have received a copy of the GNU Lesser " +
                        "General Public License along with this program. If " +
                        "not, see " +
                        "<a href=\"http://www.gnu.org/licenses\">" +
                        "http://www.gnu.org/licenses</a><br/><br/>"

                    onVisibleChanged: {
                        if (visible) {
                            controller.pausePipeline()
                        } else {
                            controller.startPipeline()
                        }
                    }
                }

                PostCapture {
                    id: postCapture
                    anchors {
                        top: about.bottom
                        topMargin: 5
                        right: parent.right
                        rightMargin: 30
                    }
                    enabled: controller.pipelineReady && file != ""
                    visible: !controller.recording && !page.__dialogsVisible
                    onClicked: controller.stopPipeline()
                    opacity: enabled ? 1 : page.__dimmedOpacity
                    Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }
                }

                Button {
                    id: shutter
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/shutter-bg.png"
                        disabledBackground: "qrc:/resources/shutter-bg.png"
                        pressedBackground: "qrc:/resources/shutter-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    anchors {
                        top: postCapture.bottom
                        topMargin: 5
                        right: parent.right
                        rightMargin: 30
                    }
                    height: 90
                    width: 100
                    iconSource: controller.recording ? "qrc:/resources/recording-icon.png" : "qrc:/resources/video-icon.png"
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady
                    opacity: enabled ? 1 : page.__dimmedOpacity
                    Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }

                    onClicked: {
                        controller.shutterClicked()
                    }
                }

                Button {
                    id: cameraShutter
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/shutter-bg.png"
                        disabledBackground: "qrc:/resources/shutter-bg.png"
                        pressedBackground: "qrc:/resources/shutter-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    anchors {
                        top:  postCapture.bottom
                        topMargin: 5
                        right: shutter.left
                        rightMargin: 10
                    }
                    height: 90
                    width: 100
                    iconSource: "qrc:/resources/camera-icon.png"
                    visible:  controller.recording ? false: !page.__dialogsVisible
                    enabled: controller.pipelineReady
                    opacity: enabled ? 1 : page.__dimmedOpacity
                    Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }

                    onClicked: {
                        controller.cameraShutterClicked()
                    }
                }

                Image {
                    id: timer
                    anchors {
                        top: shutter.bottom
                        topMargin: 5
                        left: shutter.left
                        leftMargin: 10
                    }
                    source: "qrc:/resources/timer-bg.png"
                    Text {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        visible: controller.recording
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: controller.recordedTime
                    }
                    visible: controller.recording ? !page.__dialogsVisible : false
                }

                Rectangle {
                    id: deviceConf
                    anchors {
                        right: parent.right
                        rightMargin: 30
                        bottom: colorFiltersText.top
                    }
                    color: "transparent"
                    height: 40
                    width: 198
                    visible: !controller.recording && !page.__dialogsVisible

                    Text {
                        id: back
                        anchors {
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                        }
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: "Back"
                    }

                    Text {
                        id: front
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: "Front"
                    }

                    MouseArea {
                        id: deviceSwitch
                        anchors {
                            left: back.right
                            right: front.left
                            verticalCenter: parent.verticalCenter
                        }
                        height: 50
                        enabled: controller.pipelineReady
                        opacity: enabled ? 1 : page.__dimmedOpacity
                        Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/resources/switch-bg.png"
                        }
                        Image {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
                                horizontalCenterOffset: controller.device ? 15 : -15
                            }
                            source: deviceSwitch.pressed ? "qrc:/resources/switch-handle-pressed.png" : "qrc:/resources/switch-handle.png"
                        }
                        onClicked: {
                            if (controller.device)
                               controller.device = 0
                            else
                               controller.device = 1
                        }
                    }
                }

                Text {
                    id: colorFiltersText
                    anchors {
                        left: colorFilterConf.left
                        bottom: colorFilterConf.top
                    }
                    visible: !page.__dialogsVisible
                    font.pixelSize: UIConstants.FONT_DEFAULT
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Color"
                }

                Button {
                    id: colorFilterConf
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/button-bg.png"
                        disabledBackground: "qrc:/resources/button-bg.png"
                        pressedBackground: "qrc:/resources/button-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    anchors {
                        right: parent.right
                        rightMargin: 30
                        bottom: filtersText.top
                    }
                    height: 50
                    width: 198
                    text: colorFilters.effectNames[controller.colorFilter]
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady
                    opacity: enabled ? 1 : page.__dimmedOpacity
                    Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }

                    onClicked: colorFilters.show()
                }

                Text {
                    id: filtersText
                    anchors {
                        left: filtersConf.left
                        bottom: filtersConf.top
                    }
                    visible: !page.__dialogsVisible
                    font.pixelSize: UIConstants.FONT_DEFAULT
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Effect"
                }

                Button {
                    id: filtersConf
                    platformStyle: ButtonStyle {
                        background: "qrc:/resources/button-bg.png"
                        disabledBackground: "qrc:/resources/button-bg.png"
                        pressedBackground: "qrc:/resources/button-bg-pressed.png"
                        textColor: "white"
                        fontPixelSize: UIConstants.FONT_DEFAULT
                        fontFamily: UIConstants.FONT_FAMILY
                    }
                    anchors {
                        right: parent.right
                        rightMargin: 30
                        bottom: parent.bottom
                        bottomMargin: 30
                    }
                    height: 50
                    width: 198
                    text: controller.videoEffect
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady
                    opacity: enabled ? 1 : page.__dimmedOpacity
                    Behavior on opacity { NumberAnimation { duration: page.__animationDuration } }
                    onClicked: effects.show()
                }
            }

            Controller {
                id: controller
                onRecordingChanged:
                    console.debug("recording changed to " + recording)
                onVideoEffectChanged:
                    console.debug("video effect changed to " + effectName)

                property bool __completed: false

                Component.onCompleted: {
                    console.debug("controller loaded, setting " + videoEffect +
                                  " as default to the effects page")
                    effects.initialEffect = videoEffect
                    colorFilters.initialEffect =
                        colorFilters.effectNames[colorFilter]
                    __completed = true
                }
            }

            Effects {
                id: effects
                effectNames: effectManager.effectNames()
            }

            Binding {
                target: controller
                property: "videoEffect"
                value: effects.selectedEffectName
                when: controller.__completed
            }

            Effects {
                id: colorFilters
                effectNames: [ "Normal", "Sepia", "Negative", "Grayscale",
                               "Natural", "Vivid", "Color Swap", "Solarize" ]
            }

            Binding {
                target: controller
                property: "colorFilter"
                value: colorFilters.selectedEffectIndex
                when: controller.__completed
            }

            Binding {
                target: postCapture
                property: "file"
                value: controller.savedFileName
                when: controller.__completed
            }

            ScreenSaver {
                id: screenSaver
                screenSaverDelayed: controller.recording
            }
        }
    }
}
