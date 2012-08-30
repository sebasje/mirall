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

Column {
    id: loginWidget
    property alias serverUrl: serverEdit.text
    property string user
    property string status
    spacing: 24

    Grid {
        id: formGrid


        columns: 2
        rows: 3
        spacing: 12
        //width: 200
        //anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width

        PlasmaComponents.Label {
            text: i18n("Server address:")
            horizontalAlignment: Text.AlignRight
            //anchors.right: parent.horizontalCenter
            //anchors.rightMargin: 24
        }
        PlasmaComponents.TextField {
            id: serverEdit
            text: "http://owncloud.vizZzion.org"
            //echoMode: TextInput.Password
            Keys.onReturnPressed: userNameEdit.forceActiveFocus()
            Keys.onTabPressed: userNameEdit.forceActiveFocus()
        }


        PlasmaComponents.Label {
            text: i18n("User name:")
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 24
        }
        PlasmaComponents.TextField {
            id: userNameEdit
            Keys.onTabPressed: passwordEdit.forceActiveFocus()
        }

        PlasmaComponents.Label {
            text: i18n("Password:")
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: 24
        }
        PlasmaComponents.TextField {
            id: passwordEdit
            echoMode: TextInput.Password
            Keys.onReturnPressed: loginWidget.acceptConfig()
            Keys.onTabPressed: buttonsRow.forceActiveFocus()
        }
    }
    PlasmaComponents.Button {
        id: buttonsRow
        text: i18n("Sign in")
        width: 80
        enabled: userNameEdit.text != "" && serverUrl != ""
        anchors {
            topMargin: 20
        }
        x: passwordEdit.x + 2
        onClicked: loginWidget.acceptConfig()
    }
    function acceptConfig() {
        var s = serverUrl;
        var u = userNameEdit.text;
        var p = passwordEdit.text;
        print(" Writing config: ------------------- " );
        print("     user: " + u);
        print("     surl: " + s);
        print("     pass: " + p);
        owncloudSettings.setupOwncloud(s, u, p);
    }

}