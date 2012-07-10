// -*- coding: iso-8859-1 -*-
/*
 *   Copyright 2011 Sebastian Kügler <sebas@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras


Item {
    id: owncloudModule
    objectName: "owncloudModule"

//     OwncloudSettings {
//         id: owncloudSettings
//     }

    width: 800; height: 500

//     MobileComponents.Package {
//         id: owncloudPackage
//         name: "org.kde.active.settings.owncloud"
//     }
// 
    Column {
        id: titleCol
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.topMargin: 20
        PlasmaExtras.Title {
            text: i18n("ownCloud")
            opacity: 1
        }
        PlasmaComponents.Label {
            id: descriptionLabel
            text: "synching etc."
            opacity: .4
        }

        FolderList {
            id: folderList
            width: parent.width
            height: 400
        }
    }


    Component.onCompleted: {
        print("Loaded OwncloudPlasmoid.qml successfully.");
    }

}
