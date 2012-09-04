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

Column {
    id: loginWidget
    property alias serverUrl: serverEdit.text
    property string user
    property string status
    spacing: 12

    Item {
        id: formGrid


//         columns: 2
//         rows: 3
//         spacing: 12
        //width: 200
        //anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        //height: childrenRect.height

        PlasmaComponents.Label {
            text: i18n("Server address:")
            horizontalAlignment: Text.AlignRight
            anchors { right: parent.horizontalCenter; rightMargin: 12; verticalCenter: serverEdit.verticalCenter; left: parent.left; }
        }
        PlasmaComponents.TextField {
            id: serverEdit
            text: "http://localhost/owncloud"
            anchors { left: parent.horizontalCenter; leftMargin: 12; top: parent.top; right: parent.right; }
            Keys.onReturnPressed: userNameEdit.forceActiveFocus()
            Keys.onTabPressed: userNameEdit.forceActiveFocus()
        }


        PlasmaComponents.Label {
            text: i18n("User name:")
            horizontalAlignment: Text.AlignRight
            anchors { right: parent.horizontalCenter; rightMargin: 12; verticalCenter: userNameEdit.verticalCenter; left: parent.left; }
        }
        PlasmaComponents.TextField {
            id: userNameEdit
            anchors { left: parent.horizontalCenter; leftMargin: 12; top: serverEdit.bottom; right: parent.right; topMargin: 12; }
            Keys.onTabPressed: passwordEdit.forceActiveFocus()
        }

        PlasmaComponents.Label {
            text: i18n("Password:")
            horizontalAlignment: Text.AlignRight
            anchors { right: parent.horizontalCenter; rightMargin: 12; verticalCenter: passwordEdit.verticalCenter; left: parent.left; }
        }
        PlasmaComponents.TextField {
            id: passwordEdit
            echoMode: TextInput.Password
            anchors { left: parent.horizontalCenter; leftMargin: 12; top: userNameEdit.bottom; right: parent.right; topMargin: 12; }
            Keys.onReturnPressed: loginWidget.acceptConfig()
            Keys.onTabPressed: buttonsRow.forceActiveFocus()
        }
        PlasmaComponents.Button {
            id: buttonsRow
            text: i18n("Sign in")
            //width: 80
            enabled: userNameEdit.text != "" && serverUrl != ""
            anchors { left: parent.horizontalCenter; leftMargin: 12; top: passwordEdit.bottom; right: parent.right; topMargin: 12; }
            onClicked: loginWidget.acceptConfig()
        }
        PlasmaComponents.Label {
            text: i18n("Wrong username or password.")
            visible: owncloudSettings.error == OwncloudSettings.AuthenticationError
            anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; }
        }
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