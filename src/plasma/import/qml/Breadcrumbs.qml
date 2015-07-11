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
    id: breadcrumbs
    width: 200
    height: iconSize
    anchors.verticalCenter: parent.verticalCenter

    property string url: dirModel.url
    //property int offset

    function urlElement(ix) {
        var offset = 3;
        var e;
        var els = currentPath.split("/");
        if (url.indexOf("webdav") == 0) {
            offset = 0;
            els = currentPath.split("files/webdav.php")[1].split("/");
//             print ("elslength: " + els.length + ";" +currentPath)
            if (els.length  <= 1 && ix == 0) {
                return " "
            }
        }
//         print(" +++ UUUU " + currentPath + " o " + offset + " ix: " + ix);
//         for (var i = offset; i < els.length; i++) {
// //             print(" ix: " + ix + " e[" + i + "] = " + els[i]);
//         }
        if (els.length < ix+offset) {
            return "/";
        }
        e = els[ix+offset];
        if (!e) {
            e = "";
        }
//         print("===> " + e);
        return e;
    }

    function targetUrl(ix) {
        var us = url.split("://");
        var protocol = us[0];
        var o = protocol+":/";
        var offset;
        if (protocol != "webdav") {
            offset = 0;
        } else {
            offset = 3;
            o = protocol+":/"; // FIXME
        }
        var els = us[1].split("/");
//         var els = url.split("/");
        for (var i = offset; i <= ix+1; i++) {
//             print(protocol + " offset, ix: " + offset + " " + ix + " e[" + i + "] = " + els[i]);
            o = o + "/" + els[i];
        }
//         e = els[ix+offset];
//         print(" targetUrl: " + o);
        return o;
    }


    ListView {
        id: crumbsList
        orientation: Qt.Horizontal
        anchors.fill: parent
        interactive: contentWidth > width
        model: {
            var us = url.split("://");
            var protocol = us[0];
            var els = us[1].split("/");
            print(" +++ URL " + url + " " + els.length);
            var o = new Array();
            var n = 0;
            var offset;
            if (protocol != "webdav") {
                offset = 1;
            } else {
                els = currentPath.split("files/webdav.php")[1].split("/");
                //print(" webdav @@@ offset: " + offset + " " + currentPath + " " + els[offset-5]);
                offset = 1;
                //o = protocol+":/"; // FIXME
            }
            for (var i = offset; i <= els.length; i++) {
//                 print(" +++++++++++ model: " + i + " o[" + n + "] = " + els[i]);
                o[n] = els[i];
                n++;
            }
            return o;
        }
        header: KQuickControlsAddons.QIconItem {
                icon: "folder-sync"
            }
        delegate: MouseArea {
            //property string targetUrl
            height: iconSize
            visible:  lbl.text != ""
            width: lbl.paintedWidth + 12
            PlasmaComponents.Label {
                id: dif
                text: lbl.text == "" ? "" : " / "
            }
            PlasmaComponents.Label {
                id: lbl
                text: urlElement(index)
                anchors { left: dif.right }
            }
            onClicked: {
                var t = targetUrl(index);
                print("GOTO: " + t);
                dirPicker.url = t;
            }
        }
    }

    Component.onCompleted: print("Breadcrumbs loaded.")
}