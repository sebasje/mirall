/***************************************************************************
 *                                                                         *
 *   Copyright 2013 Sebastian Kügler <sebas@kde.org>                       *
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
//import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
//import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.owncloud 1.0

Item {
    id: progressItem

    property QtObject progress: undefined

    function mb(bytes) {
        var mbyte = (bytes / 1024 / 1024);
        mbyte = mbyte.toFixed(2);
        return  mbyte;
    }

    function parseFile(fname) {
        var f = "";
        var pcs = fname.split("/");
        if (fname.indexOf("owncloud:/") == 0) {
            var pcs = fname.split("/");
            f = i18n("Uploading %1...", pcs[pcs.length-1]);
        } else if (fname.length) {
            f = i18n("Downloading %1...", pcs[pcs.length-1]);
        }
        return f;
    }

    Connections {
        target: progress
        onProgressChanged: {
            print("------------------");
            print("Progress changed: ");
            print("     folder: " + progress.folder);
            print("    rawfile: " + progress.file);
            print("       file: " + parseFile(progress.file));
            print("       rate: " + mb(progress.current) + "/" + mb(progress.total));
        }

    }

    PlasmaComponents.Label {
        id: syncStatusLabel
        text: parseFile(progress.file)
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

    }

    PlasmaComponents.Label {
        id: mblabel
        text: mb(progress.current) + " / " + mb(progress.total) + " Mib";
        visible: progress.total
        anchors {
            top: parent.top
            //left: parent.left
            right: parent.right
        }

    }

    PlasmaComponents.ProgressBar {
        anchors {
            top: syncStatusLabel.bottom
            left: parent.left
            right: parent.right
        }
        visible: progress.total

        maximumValue: progress.total
        value: progress.current
    }

    Component.onCompleted: {
        print(" TEST: " + parseFile("owncloud://pi/cloud/remote.php/webdav/InstantUpload/IMG_4675.JPG"))
    }

}

