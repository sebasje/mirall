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
//import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.plasma.owncloud 0.1

PlasmaComponents.ListItem {
    id: fileDelegate
    width: parent.width - 24
    height: iconSize*1.5

    state: "expanded"

    property alias op: exProxy.opacity

    states: [
        State {
            name: "default"
            PropertyChanges { target: exProxy; opacity: 0.0; }
        },
        State {
            name: "expanded"
            PropertyChanges { target: exProxy; opacity: 1.0; }
        }
    ]

    Item { id: exProxy; opacity: 0 }

    KQuickControlsAddons.QIconItem {
        id: fileIcon
        icon: iconName
        width: iconSize
        height: width
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 12; }

    }

    PlasmaComponents.Label {
        id: nameLabel
        elide: Text.ElideMiddle
        //horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        maximumLineCount: 3
        //width: parent.width
        font.pointSize: theme.smallestFont.pointSize
        anchors { verticalCenter: parent.verticalCenter; left: fileIcon.right; leftMargin: 8; right: parent.right; }
        text: name
    }
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            if (fileDelegate.state == "expanded") {
                dirView.currentIndex = index;
                var p = currentPath+"/"+name;
                //dirModel.url = p;
                open(p);
            } else {
                fileDelegate.state = "expanded";
            }
        }
    }
//     PlasmaComponents.ToolButton {
//         id: pickFolderButton
// //         text: i18n("Choose")
//         iconSource: "dialog-ok-apply"
//         anchors { verticalCenter: parent.verticalCenter; right: parent.right; }
//         opacity: op
//         onClicked: {
//             var p = currentPath+"/"+name
//             dirModel.url = p;
//             print("picking: " + p + "|"+name+"|");
//             //addSyncFolder.localFolder = currentPath;
//             directoryPicked(p);
//             nextPage();
//         }
//     }
    //Rectangle { anchors.fill: parent; color: "green"; opacity: 0.2 }
}

