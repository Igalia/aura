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
        property int animationDuration: 150
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
                id: viewFinderColorkeyPainter
                anchors.fill: parent
                color: "#080810"
                z: controller.pipelineReady ? 0 : 1

                Image {
                    anchors.fill: parent
                    source: "qrc:/resources/aura-bg.png"
                    visible: !platformWindow.active ||
                        controller.pipelineStarting
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
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
                        verticalCenter: parent.verticalCenter
                    }
                    visible: controller.pipelineStarting
                    font.pixelSize: UIConstants.FONT_XLARGE
                    font.family: UIConstants.FONT_FAMILY
                    color: "white"
                    text: "Loading..."
                }
                Component.onCompleted:
                    console.debug("viewfinder colorkey painted")
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
                animationDuration: page.animationDuration
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
                animationDuration: page.animationDuration
                effectNames: [ "Normal", "Sepia", "Negative", "Grayscale",
                               "Natural", "Vivid", "Color Swap", "Solarize" ]
            }

            Binding {
                target: controller
                property: "colorFilter"
                value: colorFilters.selectedEffectIndex
                when: controller.__completed
            }

            ToolIcon {
                id: shutter
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                iconId: controller.recording ? "camera-ongoing-recording" :
                    "camera-shutter"
                opacity: page.__dialogsVisible ? 0 : 1
                visible: opacity > 0
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                onClicked: {
                    console.debug("shutter clicked")
                    controller.shutterClicked()
                }
            }

            ToolIcon {
                id: colorFilterConf
                anchors {
                    right: parent.right
                    bottom: filtersConf.top
                }
                iconId: "camera-filter-solarize-screen"
                opacity: page.__dialogsVisible || controller.recording ? 0 : 1
                visible: opacity > 0
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                onClicked: colorFilters.show()
            }

            ToolIcon {
                id: filtersConf
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }
                iconId: "camera-high-contrast"
                opacity: page.__dialogsVisible || controller.recording ? 0 : 1
                visible: opacity > 0
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                onClicked: effects.show()
            }

            ToolIcon {
                id: aboutIcon
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                }
                iconSource: "file:///usr/share/maps/images/icon_about.png"
                opacity: page.__dialogsVisible || controller.recording ? 0 : 1
                visible: opacity > 0
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                onClicked: console.debug("about dialog")
            }

            PostCapture {
                id: postCapture
                anchors {
                    left: parent.left
                    bottom: aboutIcon.top
                }
                opacity: page.__dialogsVisible || controller.recording ? 0 : 1
                visible: opacity > 0 && file != "" && !controller.recording
                enabled: visible
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                onClicked: controller.stopPipeline()
            }

            Binding {
                target: postCapture
                property: "file"
                value: controller.savedFileName
                when: controller.__completed
            }
            Text {
                anchors {
                    right: parent.right
                    rightMargin: 10
                    bottom: parent.bottom
                    bottomMargin: 10
                }
                opacity: controller.recording ? 1 : 0
                visible: opacity > 0
                Behavior on opacity {
                    NumberAnimation { duration: page.animationDuration }
                }
                font.pixelSize: UIConstants.FONT_LARGE
                font.family: UIConstants.FONT_FAMILY
                color: "white"
                text: controller.recordedTime
            }
        }
    }
}
