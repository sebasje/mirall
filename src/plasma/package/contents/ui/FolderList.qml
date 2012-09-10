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
    objectName: "folderList"
    model: owncloudSettings.folders
    interactive: height < contentHeight
    currentIndex: -1
    spacing: 24
    delegate: FolderDelegate { }

//     footer:

    function statusIcon(e) {
        var icon = "task-complete";
        if (e == OwncloudFolder.Idle) icon = "task-complete";
        if (e == OwncloudFolder.Waiting) icon = "task-ongoing";
        if (e == OwncloudFolder.Running) icon = "task-ongoing";
        if (e == OwncloudFolder.Disabled) icon = "user-offline";
        if (e == OwncloudFolder.Error) icon = "task-reject";
        //print(" --> statusIcon for: " + statusMessage(e) + " " + e + " " + icon);
        return icon;
    }

    function statusMessage(e) {
        if (e == OwncloudFolder.Idle) return i18n("Idle");
        if (e == OwncloudFolder.Waiting) return i18n("Waiting...");
        if (e == OwncloudFolder.Running) return i18n("Syncing...");
        if (e == OwncloudFolder.Disabled) return i18n("Disabled");
        if (e == OwncloudFolder.Error) return i18n("Error");
    }

    Component.onCompleted: owncloudSettings.refresh()
}
