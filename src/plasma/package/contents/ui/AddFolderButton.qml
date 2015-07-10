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
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
//import org.kde.plasma.owncloud 0.1
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddonsComponents

MouseArea {
    id: addFolderButton


    height:  64
    width: parent.width

    KQuickControlsAddonsComponents.QIconItem {
        id: folderIcon
        icon: "folder-new"
        width: 32
        height: width
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; }
    }
    PlasmaExtras.Heading {
        level: 4
        id: aliasLabel
        text: i18n("Choose Folder...")
        anchors { top: folderIcon.top; left: folderIcon.right; right: parent.right; leftMargin: 12; }
    }
    PlasmaComponents.Label {
        id: infoLabel
        font.pointSize: theme.smallestFont.pointSize
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignTop
        anchors { left: aliasLabel.left; right: parent.right; top: aliasLabel.bottom; bottom: parent.bottom; }
        text: i18n("Click to add a customer folder")
        opacity: 0.6
    }
}