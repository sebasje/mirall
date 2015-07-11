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
import org.kde.plasma.owncloud 1.0


Item {
    id: loginWidget
    property alias serverUrl: serverEdit.text
    property string user
    property string status

    Item {
        id: aligner
        property int maxWidth: 300
        anchors { top: parent.top; }
        x: parent.width / 3
        width: (parent.width-x) > maxWidth ? maxWidth : parent.width-x
        height: 400
    }
    width: parent.width

    PlasmaComponents.Label {
        text: i18n("Server address:")
        horizontalAlignment: Text.AlignRight
        anchors { right: aligner.left; rightMargin: 12; verticalCenter: serverEdit.verticalCenter; left: parent.left; }
    }
    PlasmaComponents.TextField {
        id: serverEdit
        text: "http://localhost/owncloud"
        anchors { left: aligner.left; leftMargin: 12; top: parent.top; right: aligner.right; }
        Keys.onReturnPressed: userNameEdit.forceActiveFocus()
        Keys.onTabPressed: userNameEdit.forceActiveFocus()
    }

    PlasmaComponents.Label {
        text: i18n("User name:")
        horizontalAlignment: Text.AlignRight
        anchors { right: aligner.left; rightMargin: 12; verticalCenter: userNameEdit.verticalCenter; left: parent.left; }
    }
    PlasmaComponents.TextField {
        id: userNameEdit
        anchors { left: aligner.left; leftMargin: 12; top: serverEdit.bottom; right: aligner.right; topMargin: 12; }
        Keys.onTabPressed: passwordEdit.forceActiveFocus()
    }

    PlasmaComponents.Label {
        text: i18n("Password:")
        horizontalAlignment: Text.AlignRight
        anchors { right: aligner.left; rightMargin: 12; verticalCenter: passwordEdit.verticalCenter; left: parent.left; }
    }
    PlasmaComponents.TextField {
        id: passwordEdit
        echoMode: TextInput.Password
        anchors { left: aligner.left; leftMargin: 12; top: userNameEdit.bottom; right: aligner.right; topMargin: 12; }
        Keys.onReturnPressed: loginWidget.acceptConfig()
        Keys.onTabPressed: buttonsRow.forceActiveFocus()
    }
    PlasmaComponents.Button {
        id: buttonsRow
        text: i18n("Sign in")
        //width: 80
        enabled: userNameEdit.text != "" && serverUrl != ""
        anchors { left: aligner.left; leftMargin: 12; top: passwordEdit.bottom; right: aligner.right; topMargin: 12; }
        onClicked: loginWidget.acceptConfig()
    }
    PlasmaComponents.Label {
        text: i18n("Wrong username or password.")
        visible: owncloudSettings.error == OwncloudSettings.AuthenticationError
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; }
    }
    function acceptConfig() {
        var s = serverUrl;
        var u = userNameEdit.text;
        var p = passwordEdit.text;
        print(" Writing config: ------------------- " );
        print("     user: " + u);
        print("     surl: " + s);
        //print("     pass: " + p);
        owncloudSettings.setupOwncloud(s, u, p);
    }

    Behavior on opacity { FadeAnimation { } }
}
