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
import org.kde.kquickcontrolsaddons 2.0

Item {
    id: errorHandler
    objectName: "errorHandler"

    PlasmaComponents.Label {

        id: errorLabel
        anchors { top: parent.top; left: parent.left; right: parent.right; }

        text: errorMessage(owncloudSettings.error)
        visible: OwncloudSettings.NoConfigurationError != owncloudSettings.error && OwncloudSettings.AuthenticationError != owncloudSettings.error

    }

    PlasmaComponents.ToolButton {
        id: startDaemonButton
        text: i18n("Start Sync Daemon")
        iconSource: "system-run"
        visible: owncloudSettings.error == OwncloudSettings.NoDaemonError
        anchors { top: errorLabel.bottom; left: parent.horizontalCenter; }

        onClicked: owncloudSettings.startDaemon()
    }

    LoginWidget {
        visible: owncloudSettings.error == OwncloudSettings.NoConfigurationError || owncloudSettings.error == OwncloudSettings.AuthenticationError
        anchors.fill: parent
    }

    function errorMessage(e) {
        if (e == OwncloudSettings.NoError) return i18n("You are logged in.");
        if (e == OwncloudSettings.AuthenticationError) return i18n("The supplied username or password is wrong.");
        if (e == OwncloudSettings.NetworkError) return i18n("Cannot connect to ownCloud server.");
        if (e == OwncloudSettings.NoConfigurationError) return i18n("No ownCloud server configured.");
        if (e == OwncloudSettings.NoDaemonError) return i18n("The synchronization daemon is not running.");
        if (e == OwncloudSettings.CustomError) return i18n("Something unexpected happened.");
    }

    Component.onCompleted: print("Errorhandler loaded.");
}
