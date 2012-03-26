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
                    visible: controller.pipelineStarting
                    font.pixelSize: UIConstants.FONT_XLARGE
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Loading..."
                }

                MouseArea {
                    id: about
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -129
                        verticalCenter: parent.top
                        verticalCenterOffset: 60
                    }
                    height: 60
                    width: 198
                    Image {
                        anchors.fill: parent
                        source: parent.pressed ? "qrc:/resources/button-bg-pressed.png" : "qrc:/resources/button-bg.png"
                    }
                    Text {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: "About"
                    }
                    enabled: controller.pipelineReady
                    visible: !controller.recording && !page.__dialogsVisible

                    onClicked: appWindow.pageStack.push(aboutView)

                    Component {
                        id: aboutView
                        AboutView {
                            appString: "Aura"
                            iconSource: "file:///opt/aura/share/icons/hicolor/" +
                            "64x64/apps/aura.png"
                            email: "info@igalia.com"
                            twitterUrl: "https://twitter.com/#!/Igalia"
                            copyright: "Copyright © 2012 Igalia"
                            recommendBody: ""
                            license:
                                "This program is free software; you can " +
                                "redistribute it and/or modify it under the " +
                                "terms of the GNU Lesser General Public " +
                                "License as published by the Free Software " +
                                "Foundation; version 2.1 of the License, or " +
                                "(at your option) any later version." +
                                "<br/><br/>" +

                                "This program is distributed in the hope " +
                                "that it will be useful, but WITHOUT ANY " +
                                "WARRANTY; without even the implied warranty " +
                                "of MERCHANTABILITY or FITNESS FOR A " +
                                "PARTICULAR PURPOSE. See the GNU Lesser " +
                                "General Public License for more details." +
                                "<br/><br/>" +

                                "You should have received a copy of the GNU " +
                                "Lesser General Public License along with " +
                                "this program. If not, see " +
                                "<a href=\"http://www.gnu.org/licenses\">" +
                                "http://www.gnu.org/licenses</a><br/><br/>"
                        }
                    }
                }

                PostCapture {
                    id: postCapture
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -129
                        verticalCenter: parent.top
                        verticalCenterOffset: 130
                    }
                    enabled: controller.pipelineReady
                    visible: !controller.recording && !page.__dialogsVisible
                    onClicked: controller.stopPipeline()
                }

                MouseArea {
                    id: shutter
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -80
                        verticalCenter: parent.verticalCenter
                    }
                    height: 100
                    width: 100
                    Image {
                        anchors.fill: parent
                        source: parent.pressed ? "qrc:/resources/shutter-bg-pressed.png" : "qrc:/resources/shutter-bg.png"
                    }
                    Image {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        source: "qrc:/resources/video-icon.png"
                        visible: !controller.recording
                    }
                    Image {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        source: "qrc:/resources/recording-icon.png"
                        visible: controller.recording
                    }
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady

                    onClicked: {
                        controller.shutterClicked()
                    }
                }

                MouseArea {
                    id: colorFilterConf
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -129
                        verticalCenter: parent.bottom
                        verticalCenterOffset: -130
                    }
                    height: 60
                    width: 198
                    Image {
                        anchors.fill: parent
                        source: parent.pressed ? "qrc:/resources/button-bg-pressed.png" : "qrc:/resources/button-bg.png"
                    }
                    Text {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: colorFilters.effectNames[controller.colorFilter]
                    }
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady

                    onClicked: colorFilters.show()
                }

                MouseArea {
                    id: filtersConf
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -129
                        verticalCenter: parent.bottom
                        verticalCenterOffset: -60
                    }
                    height: 60
                    width: 198
                    Image {
                        anchors.fill: parent
                        source: parent.pressed ? "qrc:/resources/button-bg-pressed.png" : "qrc:/resources/button-bg.png"
                    }
                    Text {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        font.pixelSize: UIConstants.FONT_DEFAULT
                        font.family: UIConstants.FONT_FAMILY
                        color: "white"
                        text: controller.videoEffect
                    }
                    visible: !page.__dialogsVisible
                    enabled: controller.pipelineReady
                    onClicked: effects.show()
                }

                Image {
                    anchors {
                        horizontalCenter: parent.right
                        horizontalCenterOffset: -188
                        verticalCenter: parent.verticalCenter
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
                    visible: !page.__dialogsVisible
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
        }
    }
}
