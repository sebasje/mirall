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
    id: favoritesWidget

    ListModel {
        id: favoritesModel
        // Pictures, Documents, Music, Videos, InstantUpload
        ListElement {
            iconSource: "folder-image"
            localPath: "/home/sebas/ownCloud/Pictures"
            remotePath: "Pictures"
        }
        ListElement {
            iconSource: "folder-documents"
            localPath: "/home/sebas/ownCloud/Documents"
            remotePath: "Documents"
        }
        ListElement {
            iconSource: "folder-sound"
            localPath: "/home/sebas/ownCloud/Music/"
            remotePath: "Music"
        }
        ListElement {
            iconSource: "folder-video"
            localPath: "/home/sebas/ownCloud/Videos/"
            remotePath: "Videos"
        }
        ListElement {
            iconSource: "folder-download"
            localPath: "/home/sebas/ownCloud/InstantUpload"
            remotePath: "InstantUpload"
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
        if (folder == "Music") o = "";
        if (folder == "Videos") o = i18n("Movies and films");
        if (folder == "InstantUpload") o = i18n("Photos uploaded from camera");
        return o;
    }

    PlasmaExtras.Heading {
        id: favHeading
        text: i18n("What would you like to synchronize?")
        level: 3
        anchors { left: parent.left; top: parent.top; }
    }

    ListView {
        id: favoritesList
        model: favoritesModel
        anchors { left: parent.left; right: parent.right; top: favHeading.bottom; topMargin: 12; bottom: footerItem.top; }
        interactive: contentHeight > height
        clip: true

        delegate: Item {
            id: favDelegate
            height: 64
            width: parent.width

            QtExtraComponents.QIconItem {
                id: folderIcon
                icon: iconSource
                width: parent.height*0.8
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
                id: errorLabel
                font.pointSize: theme.smallestFont.pointSize
                wrapMode: Text.Wrap
                verticalAlignment: Text.AlignTop
                anchors { left: aliasLabel.left; right: addFav.left; top: aliasLabel.bottom; bottom: parent.bottom; }
                text: aliasDescription(remotePath)
                opacity: 0.4
                //opacity: (expanded && folderStatus == OwncloudFolder.Error) ? 1.0 : 0.4
                //Behavior on opacity { FadeAnimation { } }
            }
            PlasmaComponents.CheckBox {
                id: addFav
                //iconSource: "folder-add"
                anchors { verticalCenter: folderIcon.verticalCenter; right: parent.right; rightMargin: 24; }
                onClicked: {
                    var a = aliasName(remotePath);
                    if (checked) {
                        print(" --> Adding Syncfolder: " + a + " L: " + localPath + " R: " + remotePath);
                        owncloudSettings.addSyncFolder(localPath, remotePath, a);
                        //owncloudModule.state = "default";
                    } else {
                        print(" --> Removing Syncfolder: " + aliasName(remotePath) + " L: " + localPath + " R: " + remotePath);
                        owncloudSettings.removeSyncFolder(a);
                    }
                }
            }

        }
    }

    Item {
        id: footerItem
        height: 48
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; topMargin: 12; }

    }
    Behavior on opacity { FadeAnimation { } }
}
