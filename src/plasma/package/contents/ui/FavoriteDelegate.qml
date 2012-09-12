/*
 *   Copyright 2012 Sebastian Kügler <sebas@kde.org>
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
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.plasma.owncloud 0.1


Item {
    id: favDelegate

    property bool expanded: syncEnabled && (!localPathExists || !remotePathExists)
    property string errorMessage
    property string localPath: aliasLocalPath(remotePath)
    property string displayName: aliasName(remotePath)
    property bool localPathExists: dir.exists(localPath)

    height:  expanded ? 84 : 48
    width: parent.width
    //opacity: expanded ? 0.6 : 1

    QtExtraComponents.QIconItem {
        id: folderIcon
        icon: iconSource
        width: 32
        height: width
        anchors { top: parent.top; left: parent.left; }
    }
    PlasmaExtras.Heading {
        level: 4
        id: aliasLabel
        text: aliasName(remotePath)
        anchors { top: parent.top; left: folderIcon.right; right: addFav.left; leftMargin: 12; }
    }
    PlasmaComponents.Label {
        id: infoLabel
        font.pointSize: theme.smallestFont.pointSize
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignTop
        anchors { left: aliasLabel.left; right: addFav.left; top: aliasLabel.bottom; bottom: parent.bottom; }
        text: {
            if (errorMessage != "") {
                return errorMessage;
            } else {
                return addFav.checked ? aliasCheckedDescription(remotePath) : aliasDescription(remotePath);
            }
        }
        opacity: addFav.checked ? 0.6 : 0.4
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            addFav.checked = !addFav.checked;
            toggle();
        }
    }

    PlasmaComponents.CheckBox {
        id: addFav
        anchors { verticalCenter: folderIcon.verticalCenter; right: parent.right; rightMargin: 12; }
        onClicked: toggle()
    }

    PlasmaComponents.Button {
        opacity: (!remotePathExists && expanded) ? 1 : 0
        text: i18n("Create Remote Folder")
        anchors { bottom: parent.bottom; left: infoLabel.left; }
        onClicked: {
            var currentJob = owncloudSettings.createRemoteFolder(remotePath);
            currentJob.finished.connect(jobFinished);
            enabled = false;
        }

        function jobFinished(success) {
            enabled = true;
            if (success) {
                print("Remote folder created! " + remotePath);
                visible = false;
            } else {
                print("failed to create remote folder created! " + remotePath);
            }
        }
    }

    PlasmaComponents.Button {
        opacity: (!localPathExists && expanded) ? 1 : 0
        text: i18n("Create Local Folder")
        anchors { bottom: parent.bottom; left: infoLabel.horizontalCenter; }
        onClicked: {
            var ok = owncloudSettings.createLocalFolder(localPath);
            if (ok) visible = false;
            localPathExists = ok;
        }
    }

    Connections {
        target: owncloudSettings
        onOwncloudStatusChanged: {
            if (owncloudSettings.owncloudStatus == OwncloudSettings.Connected) {
                owncloudSettings.checkRemoteFolder(remotePath);
            }
        }
        onRemoteFolderExists: {
            if (folder == remotePath) {
                //print( " .. remote exists? " + remotePath + " yesno: " + exists + " index: " + index + " " + favDelegate.displayName);
                favoritesModel.setProperty(index, "remotePathExists", exists);
                errorMessage = i18n("The remote folder does not exist.");
                if (exists) {
                    errorMessage = owncloudSettings.verifyFolder(localPath, remotePath, displayName);
                    favoritesModel.setProperty(index, "folderVerified", (errorMessage == ""));
                }
            }
        }
    }
    function toggle() {
        var a = aliasName(remotePath);
        var localPath = aliasLocalPath(remotePath);
        favoritesModel.setProperty(index, "syncEnabled", addFav.checked);
        if (addFav.checked) {
            print(" --> Adding Syncfolder: " + a + " L: " + localPath + " R: " + remotePath);
        } else {
            print(" --> Removing Syncfolder: " + aliasName(remotePath) + " L: " + localPath + " R: " + remotePath);
        }
    }
    Component.onCompleted: {
        owncloudSettings.checkRemoteFolder(remotePath);
    }
}
