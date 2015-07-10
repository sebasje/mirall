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

import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.owncloud 0.1

PlasmaComponents.PageStack {
    id: root
    objectName: "root"

    property bool webdavInFileManager: false
    property bool showRemoveFolder: true
    property int defaultMargin: 8

    OwncloudSettings {
        id: owncloudSettings
    }

    DirectoryLister { id: dir }

    width: 400
    height: 400

    Column {
        id: titleCol
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        PlasmaExtras.Title {
            //text: owncloudSettings.name
            opacity: 1
        }
        PlasmaComponents.Label {
            id: descriptionLabel
            //text: owncloudSettings.description
            opacity: .4
        }
    }

    state: "default"
//     state: "addFolder"

    states: [
        State {
            name: "default"
            PropertyChanges { target: folderList; opacity: 1; }
            PropertyChanges { target: addSyncFolder; opacity: 0; }
            PropertyChanges { target: setupWizard; opacity: 0; }
        },
        State {
            name: "setup"
            PropertyChanges { target: folderList; opacity: 0; }
            PropertyChanges { target: addSyncFolder; opacity: 0; }
            PropertyChanges { target: setupWizard; opacity: 1; }
        },
        State {
            name: "addFolder"
            PropertyChanges { target: folderList; opacity: 0; }
            PropertyChanges { target: addSyncFolder; opacity: 1; }
            PropertyChanges { target: setupWizard; opacity: 0; }
        },
        State {
            name: "settings"
        }
    ]

    Item {
        id: owncloudItem
        anchors { left: parent.left; right: parent.right; top: titleCol.bottom; bottom: parent.bottom; margins: 12; }
        PlasmaExtras.Heading {
            id: headingLabel
            level: 2
            height: 48
            text: {
                if (owncloudSettings.owncloudStatus != OwncloudSettings.Connected) {
                    return i18n("Synchronization");
                } else {
                    var u = owncloudSettings.url;
                    u = u.replace("https://", "");
                    u = u.replace("http://", "");
                    return i18n("Connected to %1", u);
                }
            }
            anchors { top: parent.top; left: parent.left; right: parent.right; }
        }

        FolderList {
            id: folderList
            width: parent.width
            clip: true
            height: 540
            visible: owncloudSettings.owncloudStatus == OwncloudSettings.Connected
            anchors { top: headingLabel.bottom; topMargin: 12; left: parent.left; right: parent.right; bottom: enabledSwitch.top; }
            Behavior on opacity { FadeAnimation { } }
        }

        ProgressItem {
            id: progressItem
            progress: owncloudSettings.progress
            width: parent.width
            height: 200
            visible: owncloudSettings.globalStatus == OwncloudFolder.Running
            anchors {
                bottom: enabledSwitch.top
            }

        }


        PlasmaComponents.CheckBox {
            id: enabledSwitch
            text: i18n("Enable All")
            visible: folderList.count > 0
            opacity: folderList.opacity
            checked: owncloudSettings.globalStatus != OwncloudFolder.Disabled
            anchors { bottom: parent.bottom; left: parent.left; rightMargin: 12 }
            onClicked: owncloudSettings.enableAllFolders(checked)
        }

        SetupWizard {
            id: setupWizard
            anchors { top: folderList.top; left: parent.left; right: parent.right; bottom: folderList.bottom; }
        }
        AddSyncFolder {
            id: addSyncFolder
            clip: true
            anchors { top: folderList.top; left: parent.left; right: parent.right; bottom: folderList.bottom; }
        }
    }

    NavigationItem {
        id: navigationItem
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
    Connections {
        target: owncloudSettings
        onErrorChanged: updateState()
        onOwncloudStatusChanged: updateState()
    }

    function updateState() {
        if (owncloudSettings.owncloudStatus == OwncloudSettings.Disconnected) {
            root.state = "setup";
            setupWizard.state = "login";
            print(" ------> disconnected");
        } else {
            print(" -----> connected. Folders: " + owncloudSettings.folders.length);
            if (owncloudSettings.folders.length) {
                print(" lentth OK");
                root.state = "default";
            } else {
                print(" ==> length NAK");
                root.state = "setup";
            }
            setupWizard.state = "favorites";
        }
        if (owncloudSettings.owncloudStatus == OwncloudSettings.Error) {
            if (owncloudSettings.error == OwncloudSettings.NoDaemonError) {
                root.state = "setup";
                setupWizard.state = "error";
                print("------ > No Daemon Error!")
            } else if (owncloudSettings.error == OwncloudSettings.NoConfigurationError ||
                owncloudSettings.error == OwncloudSettings.AuthenticationError) {
                root.state = "setup";
                setupWizard.state = "login";
                print(" -----> error to login ");
            } else {
                print(" -----> all good ");
                // FIXME: remove
                root.state = "default";
                setupWizard.state = "default";
            }
        }

    }

    function addFolder() {
        print("setup");
        root.state = "setup";
    }

    Component.onCompleted: {
        print("============================= complete");
        updateState();
    }
}
