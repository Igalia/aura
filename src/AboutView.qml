/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Simón Pena Placer <spena@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Miguel Gómez <magomez@igalia.com>
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
import com.nokia.extras 1.1
import "file:///usr/lib/qt4/imports/com/meego/UIConstants.js" as UIConstants

Page {
    id: page

    property string license: 'This program is free software: you can redistribute it and/or modify ' +
        'it under the terms of the GNU General Public License as published by ' +
        'the Free Software Foundation, either version 3 of the License, or ' +
        '(at your option) any later version.<br /><br />' +

        'This package is distributed in the hope that it will be useful, ' +
        'but WITHOUT ANY WARRANTY; without even the implied warranty of ' +
        'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ' +
        'GNU General Public License for more details.<br /><br />' +

        'You should have received a copy of the GNU General Public License ' +
        'along with this program. If not, see ' +
        '<a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a><br /><br />'

    property string appString: ''
    property string iconSource: ''
    property string recommendSubject: 'Download ' + appString
    property string recommendBody: appString + ' is available at ' + storeUrl
    property string email: ''
    property string opinionSubject: appString
    property string storeUrl: ''
    property string twitterUrl: ''
    property string publisherUrl: ''
    property string copyright: ''
    property string disclaimer: ''

    property int __numItems: 0

    // This dirtyness is because of QT bug of not allowing property
    // binding on ListElements
    Component.onCompleted: modelSetValues()
    onRecommendSubjectChanged: modelSetValues()
    onRecommendBodyChanged: modelSetValues()
    onEmailChanged: modelSetValues()
    onOpinionSubjectChanged: modelSetValues()
    onStoreUrlChanged: modelSetValues()
    onTwitterUrlChanged: modelSetValues()
    onPublisherUrlChanged: modelSetValues()

    function modelSetValues() {
        console.debug("modelSetValues()")
        aboutOptions.setValues()
    }

    tools: ToolBarLayout {
        ToolIcon {
            id: backIcon
            iconId: 'toolbar-back'
            onClicked: pageStack.pop()
        }
    }

    ListModel {
        id: aboutOptions

        ListElement {
            title: 'Recommend this application'
            action: 'openExternally'
            data: ''
            visibility: true
        }
        ListElement {
            title: 'Tell us your opinion'
            action: 'openExternally'
            data: ''
            visibility: true
        }
        ListElement {
            title: 'Rate us at Nokia Store'
            action: 'openStore'
            data: ''
            visibility: true
        }
        ListElement {
            title: 'Follow us in Twitter'
            action: 'openExternally'
            data: ''
            visibility: true
        }
        ListElement {
            title: 'Other applications made by us'
            action: 'openStore'
            data: ''
            visibility: true
        }

        // This dirtyness is because of QT bug of not allowing property
        // binding on ListElements
        function setValues() {

            __numItems = 0

            if (recommendSubject != '' && recommendBody != '') {
                aboutOptions.get(0).data = 'mailto:?subject=' + recommendSubject + '&body=' + recommendBody
                aboutOptions.get(0).visibility = true
                __numItems++
            } else {
                aboutOptions.get(0).visibility = false
            }

            if (email != '' && opinionSubject != '') {
                aboutOptions.get(1).data = 'mailto:' + email + '?subject=' + opinionSubject
                aboutOptions.get(1).visibility = true
                __numItems++
            } else {
                aboutOptions.get(1).visibility = false
            }

            if (storeUrl != '') {
                aboutOptions.get(2).data = storeUrl
                aboutOptions.get(2).visibility = true
                __numItems++
            } else {
                aboutOptions.get(2).visibility = false
            }

            if (twitterUrl != '') {
                aboutOptions.get(3).data = twitterUrl
                aboutOptions.get(3).visibility = true
                __numItems++
            } else {
                aboutOptions.get(3).visibility = false
            }

            if (publisherUrl != '') {
                aboutOptions.get(4).data = publisherUrl
                aboutOptions.get(4).visibility = true
                __numItems++
            } else {
                aboutOptions.get(4).visibility = false
            }

            for (var i = 0; i < 5; i++) {
                if (aboutOptions.get(i).visibility) {
                    console.debug("aboutOptions.get(" + i + ").data: " + aboutOptions.get(i).data)
                } else {
                    console.debug("aboutOptions.get(" + i + ").visibility: " + aboutOptions.get(i).visibility)
                }
            }
        }
    }

    Flickable {
        id: flick
        clip: true
        anchors.fill: parent
        anchors {
            topMargin: UIConstants.DEFAULT_MARGIN
            leftMargin: UIConstants.DEFAULT_MARGIN
            rightMargin: UIConstants.DEFAULT_MARGIN
        }
        contentHeight: contentColumn.height

        Column {
            id: contentColumn
            spacing: UIConstants.DEFAULT_MARGIN
            width: parent.width

            Image {
                id: aboutImage
                anchors.horizontalCenter: parent.horizontalCenter
                source: iconSource
            }

            Label {
                id: aboutVersion
                text: appString
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                platformStyle: LabelStyle {
                    fontPixelSize: UIConstants.FONT_XLARGE
                }
                color: UIConstants.COLOR_FOREGROUND
            }

            Rectangle {
                width: parent.width
                height: __numItems * UIConstants.LIST_ITEM_HEIGHT_SMALL
                color: 'white'

                Column {
                    id: subcolumn
                    anchors {
                        fill: parent
                    }
                    Repeater {
                        id: repeater
                        model: aboutOptions
                        Item {
                            height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                            width: parent.width
                            visible: model.visibility
                            enabled: visible

                            BorderImage {
                                anchors.fill: parent
                                visible: mouseArea.pressed
                                source: 'image://theme/meegotouch-list-fullwidth-background-pressed-vertical-center'
                            }

                            Label {
                                anchors {
                                    left: parent.left
                                    leftMargin: UIConstants.DEFAULT_MARGIN
                                    verticalCenter: parent.verticalCenter
                                }
                                platformStyle: LabelStyle {
                                    fontPixelSize: UIConstants.FONT_SLARGE
                                }
                                text: model.title
                            }

                            MoreIndicator {
                                anchors {
                                    right: parent.right
                                    rightMargin: UIConstants.DEFAULT_MARGIN
                                    verticalCenter: parent.verticalCenter
                                }
                            }

                            Rectangle {
                                anchors.bottom: parent.bottom
                                width: parent.width
                                height: 1
                                color: UIConstants.COLOR_BUTTON_DISABLED_FOREGROUND
                                visible: index !== repeater.model.count - 1
                            }

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                onClicked: {
                                    if (model.action === 'openStore') {
                                        aboutViewController.openStoreClient(model.data)
                                    } else if (model.action === 'openExternally') {
                                        Qt.openUrlExternally(model.data)
                                    }
                                }
                            }
                        }
                    }
                }

                BorderImage {
                    id: border
                    source: 'qrc:/resources/round-corners-shadow.png'
                    anchors.fill: parent
                    border.left: 18; border.top: 18
                    border.right: 18; border.bottom: 18
                }
            }

            Label {
                id: aboutCopyright
                text: copyright
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                platformStyle: LabelStyle {
                    fontPixelSize: UIConstants.FONT_LSMALL
                    fontFamily: UIConstants.FONT_FAMILY_LIGHT
                }
                color: UIConstants.COLOR_FOREGROUND
                visible: copyright != ''
            }

            Label {
                id: aboutDisclaimer
                text: disclaimer
                width: parent.width
                horizontalAlignment: Text.AlignJustify
                platformStyle: LabelStyle {
                    fontPixelSize: UIConstants.FONT_LSMALL
                    fontFamily: UIConstants.FONT_FAMILY_LIGHT
                }
                color: UIConstants.COLOR_FOREGROUND
                onLinkActivated: Qt.openUrlExternally(link)
                visible: disclaimer != ''
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: 'License'
                onClicked: licenseDialog.open()
            }
        }
    }

    QueryDialog {
        id: licenseDialog
        message: license
        acceptButtonText: 'OK'
    }

    ScrollDecorator {
        flickableItem: flick
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
    }
}
