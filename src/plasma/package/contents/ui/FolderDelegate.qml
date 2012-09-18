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
        //print("clicked" + index);
        if (!expanded) {
            folderList.currentIndex = index;
            expanded = true;
        } else {
            folderList.currentIndex = -1;
            expanded = false;
        }
    }

    QIconItem {
        id: folderIcon
        icon: "folder-green"
        width: 48
        height: width
        anchors { top: parent.top; left: parent.left; }
        opacity: (folderStatus == OwncloudFolder.Idle) ? 1.0 : 0.8
        Behavior on opacity { FadeAnimation { } }
    }
    QIconItem {
        icon: statusIcon(folderStatus)
        width: 16
        height: 16
        anchors { bottom: folderIcon.bottom; right: folderIcon.right; }
        opacity: (folderStatus == OwncloudFolder.Running) ? 0.0 : 1.0
        Behavior on opacity { FadeAnimation { } }
    }
    PlasmaComponents.BusyIndicator {
        width: folderIcon.width/2
        height: width
        anchors { bottom: folderIcon.bottom; right: folderIcon.right; }
        opacity: (folderStatus == OwncloudFolder.Running) ? 1.0 : 0.0
        running: folderStatus == OwncloudFolder.Running
        Behavior on opacity { FadeAnimation { } }
    }
    PlasmaExtras.Heading {
        level: 4
        id: aliasLabel
        text: displayName
        anchors { top: parent.top; left: folderIcon.right; right: enabledSwitch.left; leftMargin: 12; }
    }
    PlasmaComponents.Label {
        id: errorLabel
        font.pointSize: theme.smallestFont.pointSize
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignTop
        anchors { left: aliasLabel.left; right: enabledSwitch.left; top: aliasLabel.bottom; bottom: parent.bottom; }
        text: {
            var out;
            if (folderStatus == OwncloudFolder.Error) {
                out = errorMessage;
            } else if (folderStatus == OwncloudFolder.Running) {
                out = i18n("Syncing...");
            } else {
                out = i18n("Last sync: %1", friendlyDate(syncTime));
            }
            if (expanded) {
                out = out + i18n("<br /><br />Local folder: %1", localPath);
                out = out + i18n("<br />Remote folder: %1", remotePath);
            }
            return out;
        }
        opacity: (expanded && folderStatus == OwncloudFolder.Error) ? 1.0 : 0.4
        Behavior on opacity { FadeAnimation { } }
    }
    PlasmaComponents.Switch {
        id: enabledSwitch
        checked: OwncloudFolder.Disabled != folderStatus
        anchors { verticalCenter: folderIcon.verticalCenter; right: parent.right; rightMargin: 24; }
        onClicked: {
            if (checked) {
                enable();
            } else {
                disable();
            }
        }
    }

    PlasmaComponents.ToolButton {
        id: syncFolderButton
        text: i18n("Sync Now")
        iconSource: "view-refresh"
        enabled: OwncloudFolder.Running != folderStatus
        opacity: OwncloudFolder.Disabled != folderStatus && expanded ? 1 : 0
        anchors { right: removeFolderButton.left; rightMargin: 12; bottom: parent.bottom; bottomMargin: 12; }
        Behavior on opacity { FadeAnimation { duration: expanded ? 400 : 150 } }
        onClicked: {
            print("Sync folder " + displayName);
            sync();
        }
    }

    PlasmaComponents.ToolButton {
        id: removeFolderButton
        text: i18n("Remove")
        iconSource: "list-remove"
        opacity: expanded ? 1 : 0
        anchors { right: parent.right; rightMargin: 12; bottom: parent.bottom; bottomMargin: 12; }
        Behavior on opacity { FadeAnimation { duration: expanded ? 400 : 150 } }
        onClicked: {
            print("Remove folder " + displayName);
            owncloudSettings.removeSyncFolder(displayName);
        }
    }

    function friendlyDate(date) {
        // this way to get a Date object seems to be reliable...
        var d = new Date( Qt.formatDate(date, "MM/dd/yyyy"));
        d.setMinutes( Qt.formatDateTime(date, "mm"));
        d.setHours( Qt.formatDateTime(date, "hh"));
        d.setSeconds( Qt.formatDateTime(date, "ss"));

        // ago is the difference between last sync and now in seconds
        var now = new Date();
        var ago = (now.getTime() - d.getTime()) / 1000;

        //var output = "";
        var dateFormat = ""
        if (ago >= 86400 * 365) {
            // more than a year ago, be verbose
            dateFormat = "ddd dd MMM yyyy, hh:mm";
        } else if (ago >= 86400*7) {
            // at least 1 week ago, so include day and month
            dateFormat = "ddd dd MMM, hh:mm";
        } else if (ago >= 86400) {
            // at least 24hrs ago, so include day
            dateFormat = "dddd, hh:mm";
        } else {
            // within the last 24hrs, only show the time
            dateFormat = "hh:mm";
        }
        return Qt.formatDateTime(d, dateFormat);
    }

}
