/***************************************************************************
 *                                                                         *
 *   Copyright 2012-2013 Sebastian Kügler <sebas@kde.org>                       *
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
import org.kde.plasma.owncloud 0.1


Item {
    id: owncloudModule
    objectName: "owncloudModule"

    property bool webdavInFileManager: false
    property bool showRemoveFolder: false

    anchors.margins: 24

    OwncloudSettings {
        id: owncloudSettings
        objectName: "owncloudSettings"
        signal updateGlobalStatus(int s)
        // Update the popup icon
        onGlobalStatusChanged: updateGlobalStatus(owncloudSettings.globalStatus)
    }

    DirectoryLister { id: dir }

    Item {
        id: owncloudItem
        anchors { fill: parent; margins: 12; }
        PlasmaExtras.Heading {
            id: headingLabel
            level: 2
            height: 48
            text: {
                if (owncloudSettings.owncloudStatus != OwncloudSettings.Connected) {
                    return i18n("File Synchronization");
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
            anchors { top: folderList.top; left: folderList.left; right: folderList.right; }
            visible: owncloudSettings.owncloudStatus != OwncloudSettings.Connected
        }
        PlasmaComponents.CheckBox {
            id: enabledSwitch
            text: i18n("Enable")
            visible: folderList.count > 0
            checked: owncloudSettings.globalStatus != OwncloudFolder.Disabled
            anchors { bottom: parent.bottom; left: parent.left; rightMargin: 12 }
            onClicked: owncloudSettings.enableAllFolders(checked)
        }
        PlasmaComponents.ToolButton {
            id: addSyncFolderButton
            text: i18n("Settings...")
            iconSource: "configure"
            visible: owncloudSettings.owncloudStatus == OwncloudSettings.Connected
            anchors { bottom: parent.bottom; right: parent.right; rightMargin: 12; }
            onClicked: {
                owncloudSettings.openConfig();
            }
        }
    }
}
