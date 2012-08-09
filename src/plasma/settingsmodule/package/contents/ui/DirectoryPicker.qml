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
//import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.plasma.owncloud 0.1
//import org.kde.qtextracomponents 0.1
ListView {
    id: directoryPicker
    //height: 300

    property string currentPath: "/home/sebas/tmp/"
    signal directoryPicked(string folder)

    DirectoryLister {
        id: lister
    }

    anchors.fill: parent
    model: lister.directories

    delegate: PlasmaComponents.ListItem {
        enabled: true
        PlasmaComponents.Label {
            text: {
                if (modelData != ".") {
                    return modelData;
                } else {
                    return i18n("Pick this Folder");
                }
            }
        }
        onClicked: {
            if (modelData != ".") {
                print("Enter ModelData: " + modelData);
                lister.enterDirectory(modelData);
            } else {
                directoryPicked(currentPath);
            }
        }
    }

    Component.onCompleted: lister.enterDirectory(currentPath)
}