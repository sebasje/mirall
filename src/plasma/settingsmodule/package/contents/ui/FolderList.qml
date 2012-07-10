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
    header: PlasmaExtras.Heading { level: 3; text: i18n("My Folders") }
    footer: PlasmaComponents.Label { text: "muchos Folders." }
    model: owncloudSettings.folders

    delegate: Item {
        width: 200
        height: 48
        PlasmaComponents.Label {
            text: displayName
            anchors { top: parent.top; left: parent.left; }
        }
        QIconItem {
            icon: status == OwncloudFolder.Idle ? "task-complete" : "task-ongoing"
            width: parent.height
            height: parent.height
            anchors { top: parent.top; right: parent.right; }
        }
    }
    OwncloudSettings {
        id: owncloudSettings
    }

}
