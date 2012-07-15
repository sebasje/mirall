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
import org.kde.plasma.owncloud 0.1


Item {
    id: owncloudModule
    objectName: "owncloudModule"
    anchors.margins: 24

//     OwncloudSettings {
//         id: owncloudSettings
//     }

//     width: childrenRect.width + 48
//     height: childrenRect.height + 48

//     MobileComponents.Package {
//         id: owncloudPackage
//         name: "org.kde.active.settings.owncloud"
//     }
//
    OwncloudSettings {
        id: owncloudSettings
    }
    Item {
        anchors { fill: parent; margins: 12; }
        PlasmaExtras.Heading {
            id: headingLabel
            level: 2
            height: 48
            text: i18n("ownCloud Sync")
            anchors { top: parent.top; left: parent.left; right: parent.right; }
        }

        FolderList {
            id: folderList
            width: parent.width
            //height: 240
            anchors { top: headingLabel.bottom; left: parent.left; right: parent.right; bottom: statusLabel.top; }
        }

        PlasmaComponents.Label {
            id: statusLabel
            text: owncloudSettings.statusMessage
            height: 64
            width: parent.width
            anchors { bottom: parent.bottom; left: parent.left; right: parent.right; }
        }
    }
    Component.onCompleted: {
        print("Loaded OwncloudPlasmoid.qml successfully.");
    }

}
