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
import org.kde.plasma.owncloud 1.0


Item {
    id: dirPicker

    property alias currentPath: dirModel.url
    signal directoryPicked(string folder)
    property alias url: dirModel.url

//     width: 400
     height: 400

    property int iconSize: 24
    property string previousUrl

    DirModel {
        id: dirModel
        mimeFilter: "inode/directory"
        //sortDirectoriesFirst: true
        //fileNameFilter: "*.png"
    }

    //Rectangle { anchors.fill: parent; color: "green"; opacity: 0.2 }

    PlasmaComponents.ToolBar {
        id: toolbar
        anchors { top: parent.top; left: parent.left; right: parent.right; }
        height: iconSize * 2
        tools: PlasmaComponents.ToolBarLayout {
            spacing: 8
            PlasmaComponents.ToolButton {
                iconSource: "go-up"
                onClicked: {
                    goUp();
                }
            }
            Breadcrumbs {

            }
            Row {
                PlasmaComponents.ToolButton {
                    id: pickButton
                    text: i18n("Choose")
                    iconSource: "dialog-ok-apply"
                    onClicked: {
                        print("emit " + currentPath);
                        directoryPicked(currentPath);
                        nextPage();
                    }
                }
                PlasmaComponents.ToolButton {
                    id: closeButton
                    text: i18n("Cancel")
                    iconSource: "dialog-close"
                    onClicked: {
                        addSyncFolder.state = "default";
                        setupWizard.state = "favorites";
                        root.state = "default";
                        if (addFolderDialog && typeof(addFolderDialog) != "undefined") {
                            addFolderDialog.close();
                        }
                    }
                }
            }
        }

    }

    ListView {
        id: dirView
        height: 400
        anchors { left: parent.left; top: toolbar.bottom; right: parent.right; bottom: parent.bottom; }
        model: dirModel
        clip: true

        delegate: DirPickerDelegate { }
    }

    function goUp() {
        var u = url;
        u = u.split("://")[1];
        var protocol = dirModel.url.split("://")[0];
        var ds = u.split("/");
        var np = protocol+":/";
        for (var i = 0; i < ds.length-1; i++) {
            print(" ds" + i + " " + ds[i] + " " + protocol);
            np = np+"/"+ds[i];
        }
        print("dsl == " + ds.length);
        if (ds.length<=2) {
            np = np + "//";
        }
        open(np);
    }

    function open(target) {
        dirPicker.previousUrl = dirModel.url; // FIXME: make copy instead of binding!
        print("Opening URL : " + target);
        dirModel.url = target;
    }

    Component.onCompleted: print("DirPicker loaded.")
}