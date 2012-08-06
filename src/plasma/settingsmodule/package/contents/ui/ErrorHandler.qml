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
import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.plasma.owncloud 0.1
import org.kde.qtextracomponents 0.1

Item {
    id: errorHandler
    objectName: "errorHandler"

    PlasmaComponents.Label {

        id: errorLabel
        anchors { top: parent.top; left: parent.left; right: parent.right; }

        text: errorMessage(owncloudSettings.error)

    }
    
    PlasmaComponents.ToolButton {
        id: startDaemonButton
        text: i18n("Start Sync Daemon")
        visible: owncloudSettings.error == OwncloudSettings.NoDaemonError
        anchors { top: errorLabel.bottom; right: parent.right; }

        onClicked: owncloudSettings.startDaemon()
    }
//         enum Error {
//             NoError, // We're fine
//             AuthenticationError, // owncloud server doesn't accept credentials
//             NetworkError, // server is unreachable
//             NoConfigurationError, // no configuration found
//             NoDaemonError, // owncloudsyncd is not running
//             CustomError // Anything else
//         };

    function errorMessage(e) {
        if (e == OwncloudSettings.NoError) return i18n("Everything is fine.");
        if (e == OwncloudSettings.AuthenticationError) return i18n("Your username or password is wrong.");
        if (e == OwncloudSettings.NetworkError) return i18n("Cannot connect. Check your network connection.");
        if (e == OwncloudSettings.NoConfigurationError) return i18n("No ownCloud configured.");
        if (e == OwncloudSettings.NoDaemonError) return i18n("The synchronization daemon is not running.");
        if (e == OwncloudSettings.CustomError) return i18n("Something unexpected happened.");
    }

    Component.onCompleted: print("Errorhandler loaded.");
}
