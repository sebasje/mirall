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
import org.kde.plasma.owncloud 1.0
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

Item {
    property alias currentPath: remoteFolderName.text
    property bool folderExists: false
    property string checkedFolder: ""

    signal folderPicked(string folder)

    //anchors.margins: 24
//     Rectangle { anchors.fill: parent; color: "green"; opacity: 0.2 }

    PlasmaExtras.Heading {
        id: headingItem
        level: 3
        text: i18n("Pick a remote folder...");
        anchors { left: parent.left; right: parent.right; top: parent.top; }
    }
    PlasmaExtras.Paragraph {
        id: explanation
        anchors { left: parent.left; right: parent.right; top: headingItem.bottom; topMargin: 12; }
        text: i18n("Pick a folder on the remote machine which you would like to synchronize with your device. If you would like to create a new folder on <a href=\"%1\">your ownCloud server</a>, just enter its name and hit \"Create Folder\".", owncloudSettings.url)
        onLinkActivated: Qt.openUrlExternally(link)
    }
    PlasmaComponents.TextField {
        id: remoteFolderName
        //text: addSyncFolder.remoteFolder
        anchors { left: parent.left; right: parent.right; top: explanation.bottom; topMargin: 12; }
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
        anchors { left: parent.left; right: parent.right; top: remoteFolderName.bottom; }
        PlasmaComponents.Label {
            id: createLabel
            wrapMode: Text.WordWrap
            //visible: currentPath != "" && checkedFolder != ""
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

        DirPicker {
            id: dirPicker
            anchors {top: createLabel.bottom; left: parent.left; right: parent.right; topMargin: 24; }
            onDirectoryPicked: {
                currentPath = url;
                // webdav://localhost/owncloud/files/webdav.php/
                var wurl = url.split("/files/webdav.php/")[1];

                print("Directory picked " + url + "|" + wurl);
                folderPicked(wurl);
            }
            Connections {
                target: addSyncFolder
                onStateChanged: {
                    if (addSyncFolder.state == "remoteFolder") {
                        var rurl = owncloudSettings.url+"/files/webdav.php";
                        rurl = rurl.replace("http", "webdav");
                        print( "** Remote URL : " + rurl);
                        dirPicker.url = rurl;
                    }
                }
            }
        }

        PlasmaComponents.ToolButton {
            id: createRemoteFolderButton
            anchors { right: parent.right; top: createLabel.bottom; rightMargin: 24; }
            iconSource: "folder-new"
            visible: !folderExists && remoteFolderName != "" && checkedFolder != ""
            text: i18n("Create it")
            onClicked: owncloudSettings.createRemoteFolder(currentPath);

        }
        PlasmaComponents.TextField {
            id: aliasText
            anchors { left: parent.left; top: createLabel.bottom; rightMargin: 24; }
            visible: folderExists && remoteFolderName != "" && checkedFolder != ""
            onTextChanged: {
                owncloudSettings.verifyFolder(addSyncFolder.localFolder, checkedFolder, text);
            }
        }
        PlasmaComponents.ToolButton {
            id: finishedButton
            anchors { right: parent.right; top: createLabel.bottom; rightMargin: 24; }
            visible: folderExists && remoteFolderName != "" && checkedFolder != ""
            iconSource: "dialog-ok"
            text: i18n("Finished")
            onClicked: {
                print("XXX finishedButton::aliasText.text = " + aliasText.text);
                addSyncFolder.remoteFolder = currentPath;
                if (aliasText.text != "") {
                    addSyncFolder.aliasName = aliasText.text;
                } else {
                    addSyncFolder.aliasName =  currentPath;
                }
                print("XXX finishedButton::aliasText.text = " + aliasText.text + " so: " + addSyncFolder.aliasName);
                nextPage();
            }
        }
    }

    PlasmaComponents.ToolButton {
        id: goPrev
        iconSource: "go-previous"
        anchors { left: parent.left; bottom: parent.bottom; }
        onClicked: {
            addSyncFolder.state = "localPath";
            remoteFolderItem.opacity = 0; // FIXME: shouldn't be necessary
            feedbackItem.opacity = 0; // FIXME: shouldn't be necessary
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
                    var error = owncloudSettings.verifyFolder(addSyncFolder.localFolder, checkedFolder, addSyncFolder.aliasName);
                    if (error == "") {
                        finishedButton.visible = true;
                    } else {
                        createLabel.text = error;
                    }
                }
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

    Component.onCompleted: {
//         remoteFolderName.forceActiveFocus();
        print("RemoteFolderPicker.qml completed.");
    }
}