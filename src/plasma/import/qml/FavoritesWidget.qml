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

import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddonsComponents
import org.kde.plasma.owncloud 0.1


Item {
    id: favoritesWidget

    property QtObject favorites

    ListModel {
        id: favoritesModel
        // Pictures, Documents, Music, Videos, InstantUpload
        ListElement {
            iconSource: "folder-image"
            remotePath: "Pictures"
            remotePathExists: false
            folderVerified: false
            syncEnabled: false
        }
        ListElement {
            iconSource: "folder-documents"
            remotePath: "Documents"
            remotePathExists: false
            folderVerified: false
            syncEnabled: false
        }
        ListElement {
            iconSource: "folder-sound"
            remotePath: "Music"
            remotePathExists: false
            folderVerified: false
            syncEnabled: false
        }
        ListElement {
            iconSource: "folder-video"
            remotePath: "Videos"
            remotePathExists: false
            folderVerified: false
            syncEnabled: false
        }
        ListElement {
            iconSource: "folder-downloads"
            remotePath: "InstantUpload"
            remotePathExists: false
            folderVerified: false
            syncEnabled: false
        }
    }

    function aliasName(folder) {
        var o;
        if (folder == "Pictures") o = i18n("Pictures");
        if (folder == "Documents") o = i18n("Documents");
        if (folder == "Music") o = i18n("Music");
        if (folder == "Videos") o = i18n("Videos");
        if (folder == "InstantUpload") o = i18n("Instant Uploads");
        return o;
    }
    function aliasDescription(folder) {
        var o;
        if (folder == "Pictures") o = i18n("Photos and images");
        if (folder == "Documents") o = i18n("My files");
        if (folder == "Music") o = i18n("Songs on my device");
        if (folder == "Videos") o = i18n("Movies and films");
        if (folder == "InstantUpload") o = i18n("Photos uploaded from camera");
        return o;
    }
    function aliasCheckedDescription(folder) {
        return i18n("Will synchronize <b>%1</b> with the remote folder <b>%2</b>.", aliasLocalPath(folder), folder);
    }
    function aliasLocalPath(folder) {
        var o;
        if (folder == "Pictures") o = dir.picturesPath;
        if (folder == "Documents") o = dir.documentPath;
        if (folder == "Music") o = dir.musicPath;
        if (folder == "Videos") o = dir.videosPath;
        if (folder == "InstantUpload") o = dir.homePath + "InstantUpload";
        return o;
    }

//     PlasmaExtras.Heading {
//         id: favHeading
//         text: i18n("What would you like to synchronize?")
//         level: 3
//         anchors { left: parent.left; right: parent.right; top: parent.top; }
//     }
    PlasmaExtras.Paragraph {
        id: favInfo
        text: {
            if (setupWizard.isFirstRun) {
                return i18n("Choose the local directories you would like to synchronize. Click \"Finish\" to set up your folders and start the initial synchronization.");
            } else {
                return i18n("Choose the local directories you would like to synchronize.");
            }
        }
        anchors { left: parent.left; right: parent.right; top: parent.top; rightMargin: 12; }
    }

    ListView {
        id: favoritesList
        model: favoritesModel
        spacing: 12
        anchors { left: parent.left; right: parent.right; top: favInfo.bottom; topMargin: 12; bottom: footerItem.top; }
        interactive: contentHeight+48 > height
        clip: true

        delegate: FavoriteDelegate {}

        footer: AddFolderButton {
                anchors.topMargin: 48

                onClicked: {
                    setupWizard.state = "addSyncFolder";
                    addSyncFolder.state = "localFolder";
                }
        }


//         footer: Item {
//             //Rectangle { color: "orange"; opacity: 0.5; anchors.fill: parent; }
//             width: parent.width
//             height: 32
//             PlasmaComponents.ToolButton {
//                 id: addSyncFolderButton2
//                 text: i18n("Add Folder")
//                 iconSource: "list-add"
//                 anchors { bottom: parent.bottom; right: parent.right; }
//                 onClicked: {
//                     setupWizard.state = "addSyncFolder";
//                     addSyncFolder.state = "localFolder";
//                 }
//             }
//         }
    }
    PlasmaComponents.ScrollBar {
        flickableItem: favoritesList
    }


    Connections {
        target: navigationItem
        onApply: apply();
    }

    function apply() {
        print(" apply! ");

        for (i=0; i < favoritesModel.count; i++) {
            var fav = favoritesModel.get(i);
            var s = fav.syncEnabled;

            if (s) {
                var ok = true;
                var e = "";
                var r = fav.remotePath;
                var l = aliasLocalPath(r);
                var a = aliasName(r);
                if (!fav.folderVerified) {
                    print("Folder not verified: " + l + " " + r);
                    ok = false;
                }
                if (!dir.exists(l)) {
                    print("Local path does not exist: " + l);
                    ok = false;
                }
                if (ok) {
                    print("   * " + r + " " + l + "sync? " + s);
                    owncloudSettings.addSyncFolder(l, r, a);
                }
            }
        }

        return;
    }

    Item {
        id: footerItem
        height: 0
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; rightMargin: 12; }

    }
    Behavior on opacity { FadeAnimation { } }
}
