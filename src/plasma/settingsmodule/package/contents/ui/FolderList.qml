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
import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.plasma.owncloud 0.1
import org.kde.qtextracomponents 0.1

ListView {
    id: folderList
    header: PlasmaExtras.Heading { level: 3; text: i18n("My Folders"); height: 64 }
    footer: PlasmaComponents.Label { text: "muchos Folders." }
    model: owncloudSettings.folders
    interactive: height < contentHeight

    delegate: Item {
        width: parent.width
        height: 48
        PlasmaComponents.Switch {
            id: enabledSwitch
            checked: OwncloudFolder.Disabled != status
            anchors { top: parent.top; left: parent.left; rightMargin: 12 }
            onClicked: {
                if (checked) {
                    enable();
                } else {
                    disable();
                }
            }
        }
        PlasmaExtras.Heading {
            level: 4
            text: displayName
            anchors { top: parent.top; left: enabledSwitch.right; leftMargin: 12; }
        }
        QIconItem {
            icon: statusIcon(status)
            width: parent.height/2
            height: parent.height/2
            anchors { top: parent.top; right: parent.right; }
        }
    }

    OwncloudSettings {
        id: owncloudSettings
    }

    function statusIcon(e) {
        if (e == OwncloudFolder.Idle) return "task-complete";
        if (e == OwncloudFolder.Waiting) return "task-ongoing";
        if (e == OwncloudFolder.Disabled) return "user-offline";
        if (e == OwncloudFolder.Error) return "task-reject";
    }

    function statusMessage(e) {
        if (e == OwncloudFolder.Idle) return i18n("Idle");
        if (e == OwncloudFolder.Waiting) return i18n("Syncing...");
        if (e == OwncloudFolder.Disabled) return i18n("Disabled");
        if (e == OwncloudFolder.Error) return i18n("Error");
    }
}
