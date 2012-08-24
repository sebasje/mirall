/***************************************************************************
 *                                                                         *
 *   Copyright 2012 Sebastian Kügler <sebas@kde.org>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
//import org.kde.plasma.extras 0.1 as PlasmaExtras
//import org.kde.plasma.owncloud 0.1
//import org.kde.qtextracomponents 0.1
Item {
    id: addSyncFolder

    property string localFolder: ""
    property string remoteFolder: ""
    property string aliasName: ""
    height: collapsedHeight

    property int cnt: 0
    property int expandedHeight: 200
    property int collapsedHeight: 32
    property int fadingDuration: 400


    Behavior on height { NumberAnimation { duration: addSyncFolder.fadingDuration; easing.type: Easing.InOutExpo; } }

    states: [
        State {
            name: "default"
            PropertyChanges { target: addSyncFolder; height: collapsedHeight; }
            PropertyChanges { target: addSyncFolderButton; opacity: 1; }
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "localFolder"
            PropertyChanges { target: addSyncFolder; height: expandedHeight; }
            PropertyChanges { target: addSyncFolderButton; opacity: 0; }
            PropertyChanges { target: localFolderItem; opacity: 1; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "remoteFolder"
            PropertyChanges { target: addSyncFolder; height: expandedHeight; }
            PropertyChanges { target: addSyncFolderButton; opacity: 0; }
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 1; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "feedback"
            PropertyChanges { target: addSyncFolder; height: expandedHeight; }
            PropertyChanges { target: addSyncFolderButton; opacity: 0; }
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
            PropertyChanges { target: feedbackItem; opacity: 1; }
        }
    ]

    PlasmaComponents.ToolButton {
        id: addSyncFolderButton
        text: i18n("Add Folder")
        iconSource: "list-add"
        anchors { top: parent.top; left: parent.left; }
        onClicked: {
            addSyncFolder.state = "localFolder"
            directoryPickerDialog.open()
            //return;
        }
        Behavior on opacity { NumberAnimation { duration: addSyncFolder.fadingDuration; easing.type: Easing.InOutExpo; } }
    }

    function nextPage() {
        print("Next page" + addSyncFolder.state);
        if (addSyncFolder.state == "localFolder") {
            addSyncFolder.state = "remoteFolder";
            directoryPickerLoader.source = "RemoteFolderPicker.qml";
        } else if (addSyncFolder.state == "remoteFolder") {
            addSyncFolder.cnt = addSyncFolder.cnt + 1
            var localFolder = addSyncFolder.localFolder;
            var remoteFolder = "test" + cnt;
            var aliasName = addSyncFolder.aliasName;
            print("Adding folder " + localFolder + " " + remoteFolder + " " + aliasName );
            owncloudSettings.addSyncFolder(localFolder, remoteFolder, aliasName);
            addSyncFolder.state = "feedback";
            directoryPickerDialog.close();
        }
    }
    Item {
        id: localFolderItem
        anchors.fill: parent
        clip: true
        Behavior on opacity { NumberAnimation { duration: addSyncFolder.fadingDuration; easing.type: Easing.InOutExpo; } }
        PlasmaComponents.ToolButton {
            id: localFolderButton
            text: i18n("Pick Local Folder")
            iconSource: "folder-green"
            height: collapsedHeight
            anchors { top: parent.top; left: parent.left; }
            onClicked: nextPage();
        }
        PlasmaComponents.CommonDialog {
            id: directoryPickerDialog
            titleText: i18n("Pick Local Folder")
            buttonTexts: [i18n("Cancel")]
//             onButtonClicked: {
//                 //close();
//             }
            content: Loader {
                id: directoryPickerLoader
                width: theme.defaultFont.mSize.width*22
                height: theme.defaultFont.mSize.height*25
            }
            onStatusChanged: {
                if (status == PlasmaComponents.DialogStatus.Open) {
                    print("Opening dialog...");
                    directoryPickerLoader.source = "DirectoryPicker.qml"
                    //directoryPickerLoader.item.focusTextInput();
                }
            }
        }
    }
    PlasmaComponents.ToolButton {
        id: remoteFolderItem

        text: i18n("Pick Remote Folder")
        iconSource: "folder-sync"
        anchors { top: parent.top; left: parent.left; }
        onClicked: {
            addSyncFolder.cnt = addSyncFolder.cnt + 1
            var localFolder = addSyncFolder.localFolder;
            var remoteFolder = "test" + cnt;
            var l = addSyncFolder.localFolder.split('/');
            var aliasName = l[l.length];
            print("Adding folder " + localFolder + " " + remoteFolder + " " + aliasName );
            owncloudSettings.addSyncFolder(localFolder, remoteFolder, aliasName);

            addSyncFolder.state = "feedback";
            return;
        }
        Behavior on opacity { NumberAnimation { duration: addSyncFolder.fadingDuration; easing.type: Easing.InOutExpo; } }
    }

    Item {
        id: feedbackItem
        anchors.fill: parent
        PlasmaComponents.Label {
            id: feedbackLabel
            text: "The folder has been set up."
            anchors { top: parent.top; left: parent.left; bottom: parent.bottom; }

            Behavior on opacity { NumberAnimation { duration: addSyncFolder.fadingDuration; easing.type: Easing.InOutExpo; } }
        }
        Timer {
            id: feedbackTimer
            interval: 2000
            onTriggered: addSyncFolder.state = "default"
        }
    }
    onStateChanged: {
        print("State changed to: " + state);
        if (state == "feedback") {
            print("Starting timer");
            feedbackTimer.start();
        }
    }

    Component.onCompleted: state = "default";
}