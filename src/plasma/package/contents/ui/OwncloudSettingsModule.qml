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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.plasma.mobilecomponents 0.1 as MobileComponents
import org.kde.active.settings 0.1
import org.kde.plasma.owncloud 0.1

PlasmaComponents.PageStack {
    id: owncloudModule
    objectName: "owncloudModule"

    OwncloudSettings {
        id: owncloudSettings
    }

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
            //anchors.bottomMargin: 20
        }
    }

    Item {
        id: owncloudItem
        anchors { left: parent.left; right: parent.right; top: titleCol.bottom; bottom: parent.bottom; margins: 12; }
        PlasmaExtras.Heading {
            id: headingLabel
            level: 2
            height: 48
            text: {
                if (owncloudSettings.owncloudStatus != OwncloudSettings.Connected) {
                    return i18n("ownCloud Sync");
                } else {
                    var u = owncloudSettings.url;
                    u = u.replace("https://", "");
                    u = u.replace("http://", "");
                    return u;
                }
            }
            anchors { top: parent.top; left: parent.left; right: parent.right; }
        }

        FolderList {
            id: folderList
            width: parent.width
            clip: true
            height: 500
            visible: owncloudSettings.owncloudStatus == OwncloudSettings.Connected
            anchors { top: headingLabel.bottom; topMargin: 12; left: parent.left; right: parent.right; bottom: enabledSwitch.top; }
        }

        ErrorHandler {
            id: ocStatus
            //height: 400
            anchors { top: folderList.top; left: folderList.left; right: folderList.right; }
            visible: owncloudSettings.owncloudStatus != OwncloudSettings.Connected
            //Rectangle { color: "green"; anchors.fill: parent; opacity: .4; }
        }
        MiniJobItem {
            anchors { left: folderList.left; right: folderList.right; bottom: folderList.bottom; }
            height: 64
        }
        
        PlasmaComponents.Switch {
            id: enabledSwitch
            text: i18n("All Folders")
            visible: folderList.count > 0
            checked: owncloudSettings.globalStatus != OwncloudFolder.Disabled
            anchors { bottom: parent.bottom; left: parent.left; rightMargin: 12 }
            onClicked: owncloudSettings.enableAllFolders(checked)
        }
        AddSyncFolder {
            id: addSyncFolder
            pageStack: owncloudModule
            visible: owncloudSettings.owncloudStatus == OwncloudSettings.Connected
            anchors { verticalCenter: enabledSwitch.verticalCenter; right: folderList.right; rightMargin: 12 }
        }
    }
    function addFolder() {
        owncloudModule.replace();
    }

    Component.onCompleted: {
        print("Loaded OwncloudSettingsModule.qml successfully.");
    }

}
