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
//import org.kde.plasma.owncloud 1.0
//import org.kde.kquickcontrolsaddons 2.0
Item {
    id: addSyncFolder

    property string localFolder: ""
    property string remoteFolder: ""
    property string aliasName: ""
    //height: collapsedHeight

    property Item pageStack
    property int cnt: 0
    //property int expandedHeight: 200
    //property int collapsedHeight: 32
    property int fadingDuration: 250


    //Behavior on height { FadeAnimation { } }

    states: [
        State {
            name: "default"
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "localFolder"
            PropertyChanges { target: localFolderItem; opacity: 1; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "remoteFolder"
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 1; }
            PropertyChanges { target: feedbackItem; opacity: 0; }
        },
        State {
            name: "feedback"
            PropertyChanges { target: localFolderItem; opacity: 0; }
            PropertyChanges { target: remoteFolderItem; opacity: 0; }
        }
    ]

    onStateChanged: {
        // a bit tricky: if we open the sync thing in a dialog, don't fade out
        // our folderList. If we use the overlay, fade it out.
        var o = (typeof(addFolderDialog) != "undefined") ? 1 : 0;
        if (state == "default") {
            if (typeof(root) != "undefined") root.state = "default";
        } else if (state == "localFolder") {
            //folderList.opacity = o;
        } else if (state == "remoteFolder") {
            //folderList.opacity = o;
        } else if (state == "feedback") {
            if (typeof(addFolderDialog) != "undefined") {
                addFolderDialog.close();
            }
            //folderList.opacity = 1;
            print("Starting timer");
            feedbackTimer.start();
        } else {
            //folderList.opacity = 1;
        }
    }


    function nextPage() {
        //print("Next page" + addSyncFolder.state);
        if (addSyncFolder.state == "localFolder") {
            addSyncFolder.state = "remoteFolder";

        } else if (addSyncFolder.state == "remoteFolder") {
            addSyncFolder.cnt = addSyncFolder.cnt + 1
            var localFolder = addSyncFolder.localFolder;
            var remoteFolder = addSyncFolder.remoteFolder;
            var aliasName = (addSyncFolder.aliasName != "") ? addSyncFolder.aliasName : addSyncFolder.remoteFolder;
            //print("+++ Adding folder " + localFolder + " " + remoteFolder + " " + aliasName );
            owncloudSettings.addSyncFolder(localFolder, remoteFolder, aliasName);
            addSyncFolder.state = "feedback";
        }
    }

    DirPicker {
        id: localFolderItem
        anchors.fill: parent

        onDirectoryPicked: {
            print("XXX Local Directory picked " + url);
            localFolder = url;
        }

        //clip: true
        Behavior on opacity { FadeAnimation { } }
    }
    RemoteFolderPicker {
        id: remoteFolderItem
        anchors.fill: parent
        onFolderPicked: {
            print("XXX remote Folder picked ... " + folder);
            addSyncFolder.cnt = addSyncFolder.cnt + 1
            var localFolder = addSyncFolder.localFolder;
            var remoteFolder = folder;
            //var l = addSyncFolder.localFolder.split('/');
            //var aliasName = l[l.length];
            var aliasName = (addSyncFolder.aliasName != "") ? addSyncFolder.aliasName : remoteFolder;
            print("XXX Adding folder " + localFolder + " " + remoteFolder + " " + aliasName);

            //owncloudSettings.addSyncFolder(localFolder, remoteFolder, aliasName);
            owncloudSettings.addSyncFolder(localFolder, remoteFolder, aliasName);
            addSyncFolder.state = "feedback";
        }
        Behavior on opacity { FadeAnimation { } }
    }

    Item {
        id: feedbackItem
        anchors.fill: parent
        PlasmaComponents.Label {
            id: feedbackLabel
            text: "The folder has been set up."
            anchors { top: parent.top; left: parent.left; bottom: parent.bottom; }

            Behavior on opacity { FadeAnimation { } }
        }
        Timer {
            id: feedbackTimer
            interval: 2000
            onTriggered: addSyncFolder.state = "default"
        }
    }

    Behavior on opacity { FadeAnimation { } }

    Component.onCompleted: state = "default";
}