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
import org.kde.plasma.owncloud 0.1
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

Item {
    property alias currentPath: lister.currentPath
    signal directoryPicked(string folder)

    DirectoryLister {
        id: lister
        filter: filterText.text
    }
    PlasmaExtras.Heading {
        id: headingItem
        level: 3
        text: i18n("Pick a local folder...");
        anchors { left: parent.left; right: parent.right; top: parent.top; }
    }
    PlasmaComponents.TextField {
        id: filterText
        text: "test" // FIXME : remove
        anchors { left: parent.left; right: parent.right; top: headingItem.bottom; }
        onTextChanged: {
            lister.filter = text;
            forceActiveFocus();
        }
    }
    ListView {
        id: directoryPicker

        anchors { left: parent.left; right: parent.right; top: filterText.bottom; bottom: parent.bottom; }
        model: lister.directories

        delegate: PlasmaComponents.ListItem {
            enabled: true
            KQuickControlsAddons.QIconItem {
                id: iconItem
                anchors { left: parent.left; top: parent.top; bottom: parent.bottom; }
                width: parent.height
            }
            PlasmaComponents.Label {
                anchors { left: iconItem.right; top: parent.top; bottom: parent.bottom; leftMargin: parent.height / 4}
                text: {
                    if (modelData == ".") {
                        iconItem.icon = "dialog-ok-apply"
                        return i18n("Pick " + currentPath);
                    } else if (modelData == "..") {
                        iconItem.icon = "go-up"
                        return i18n("Up");
                    } else {
                        iconItem.icon = "folder"
                        return modelData;
                    }
                }
            }
            onClicked: {
                if (modelData != ".") {
                    print("Enter ModelData: " + modelData);
                    //currentPath = lister.currentPath;
                    lister.enterDirectory(modelData);
                    filterText.text = "";
                } else {
                    print("XXX directory picked " + currentPath);
                    directoryPicked(currentPath);
                }
            }
        }
    }
    onCurrentPathChanged: {
        //localFolderButton.text = currentPath
        addSyncFolder.localFolder = currentPath;
    }
    onDirectoryPicked: {
        //addSyncFolder.state = "remoteFolder";
        nextPage();
    }
    //Rectangle { anchors.fill: parent; color: "blue"; opacity: 0.05 }

    Component.onCompleted: lister.enterDirectory(currentPath)
}