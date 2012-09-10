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
import org.kde.qtextracomponents 0.1 as QtExtras

Item {
    property alias currentPath: filterText.text
    property bool folderExists: false
    property string checkedFolder: ""

    signal folderPicked(string folder)

    anchors.margins: 24

    PlasmaExtras.Heading {
        id: headingItem
        level: 2
        text: i18n("Choose a remote folder...");
        anchors { left: parent.left; right: parent.right; top: parent.top; }
    }
    PlasmaComponents.TextField {
        id: filterText
        text: addSyncFolder.remoteFolder
        anchors { left: parent.left; right: parent.right; top: headingItem.bottom; }
        onTextChanged: {
            //lister.filter = text
            folderExists = false;
            checkedFolder = "";
            forceActiveFocus();
            checkRemoteFolderTimer.stop();
            checkRemoteFolderTimer.start();
        }
    }
    Item {
        anchors { left: parent.left; right: parent.right; top: filterText.bottom; }
        PlasmaComponents.Label {
            id: createLabel
            wrapMode: Text.WordWrap
            visible: currentPath != "" && checkedFolder != ""
            anchors { left: parent.left; right: parent.right; top: parent.top; }
            text: {
                if (checkedFolder == "") return "";
                if (!folderExists) {
                    return i18n("Folder \"%1\" does not exist.", checkedFolder);
                } else {
                    return i18n("Click finish to synchronize the remote folder \"%1\" with \"%2\" on your local device.", checkedFolder, addSyncFolder.localFolder);
                }
                return "";
            }
        }
        PlasmaComponents.ToolButton {
            id: createRemoteFolderButton
            anchors { right: parent.right; top: createLabel.bottom; rightMargin: 24; }
            iconSource: "folder-new"
            visible: !folderExists && filterText != "" && checkedFolder != ""
            text: i18n("Create it")
            onClicked: owncloudSettings.createRemoteFolder(currentPath);

        }
        PlasmaComponents.TextField {
            id: aliasText
            text: addSyncFolder.remoteFolder
            anchors { left: parent.left; top: createLabel.bottom; rightMargin: 24; }
            visible: folderExists && filterText != "" && checkedFolder != ""
            onTextChanged: {
                owncloudSettings.verifyFolder(addSyncFolder.localFolder, checkedFolder, text);
            }
        }
        PlasmaComponents.ToolButton {
            id: finishedButton
            anchors { right: parent.right; top: createLabel.bottom; rightMargin: 24; }
            visible: folderExists && filterText != "" && checkedFolder != ""
            iconSource: "dialog-ok"
            text: i18n("Finished")
            onClicked: {
                addSyncFolder.remoteFolder = currentPath;
                addSyncFolder.aliasName = currentPath;
                nextPage();
            }
        }
    }

    Connections {
        target: owncloudSettings
        onRemoteFolderExists: {
            if (folder == currentPath) {
                print( " .. remote exists? " + folder + " yesno: " + exists);
                folderExists = exists;
                checkedFolder = folder;
                if (exists) {
                    owncloudSettings.verifyFolder(addSyncFolder.localFolder, checkedFolder, addSyncFolder.aliasName);
                }
            }
        }
        onFolderVerified: {
            print( "folder verified : " + error);
            if (error == "") {
                finishedButton.visible = true;
            } else {
                createLabel.text = error;
            }
        }
    }

    Timer {
        id: checkRemoteFolderTimer
        interval: 500
        onTriggered: {
            print("Checking folder ... " + currentPath);
            owncloudSettings.checkRemoteFolder(currentPath);
        }
    }
    Rectangle { anchors.fill: parent; color: "blue"; opacity: 0.05 }

    Component.onCompleted: print(" REMOTEPICKER DONE.");
}