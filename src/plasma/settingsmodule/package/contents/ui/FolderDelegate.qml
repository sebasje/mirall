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

MouseArea {
    width: parent.width
    clip: true
    height:  expanded ? 96 : 48;
    property bool expanded: folderList.currentIndex == index

    Behavior on height {
        NumberAnimation { easing.type: Easing.InOutQuart; duration: 200 }
    }

    onClicked: {
        if (!expanded) {
            folderList.currentIndex = index;
        } else {
            folderList.currentIndex = -1;
        }
    }

    PlasmaComponents.Switch {
        id: enabledSwitch
        checked: OwncloudFolder.Disabled != folderStatus
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
        id: aliasLabel
        text: displayName
        anchors { top: parent.top; left: enabledSwitch.right; leftMargin: 12; }
    }
    PlasmaComponents.Label {
        id: errorLabel
        font.pointSize: theme.smallestFont.pointSize
        anchors { left: aliasLabel.left; right: statusIcon.left; top: aliasLabel.bottom; }
        text: errorMessage + " " + statusMessage(folderStatus)
        opacity: (expanded && folderStatus == OwncloudFolder.Error) ? 1.0 : 0.0
        Behavior on opacity {
            PropertyAnimation { easing.type: Easing.InOutQuart; duration: 400 }
        }
    }
    QIconItem {
        icon: statusIcon(folderStatus)
        width: parent.height/2
        height: parent.height/2
        anchors { top: parent.top; right: parent.right; }
        opacity: (folderStatus == OwncloudFolder.Running) ? 0.0 : 1.0
        Behavior on opacity {
            PropertyAnimation { easing.type: Easing.InOutQuart; duration: 400 }
        }
    }
    PlasmaComponents.BusyIndicator {
        width: parent.height/2
        height: parent.height/2
        anchors { top: parent.top; right: parent.right; }
        opacity: (folderStatus == OwncloudFolder.Running) ? 1.0 : 0.0
        running: folderStatus == OwncloudFolder.Running
        Behavior on opacity {
            PropertyAnimation { easing.type: Easing.InOutQuart; duration: 400 }
        }
    }
}
