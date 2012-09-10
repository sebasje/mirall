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
    id: setupWizard
    objectName: "setupWizard"
    state: "login"
    property int maxWidth: 600
    states: [
        State {
            name: "login"
            PropertyChanges { target: loginWidget; opacity: 1; }
            PropertyChanges { target: favoritesWidget; opacity: 0; }
            PropertyChanges { target: errorWidget; opacity: 0; }
        },
        State {
            name: "favorites"
            PropertyChanges { target: loginWidget; opacity: 0; }
            PropertyChanges { target: favoritesWidget; opacity: 1; }
            PropertyChanges { target: errorWidget; opacity: 0; }
        },
        State {
            name: "error"
            PropertyChanges { target: loginWidget; opacity: 0; }
            PropertyChanges { target: favoritesWidget; opacity: 0; }
            PropertyChanges { target: errorWidget; opacity: 1; }
        }
    ]

    Connections {
        target: owncloudSettings
        onOwncloudStatusChanged: {
            if (owncloudSettings.owncloudStatus != OwncloudSettings.Connected &&
                owncloudSettings.error == OwncloudSettings.NoDaemonError) {
                setupWizard.state = "error";
            } else if (owncloudSettings.owncloudStatus != OwncloudSettings.Connected) {
                setupWizard.state = "login";
            } else {
                setupWizard.state = "favorites";
            }
            print(" owncloudSettings state changed to ..." + setupWizard.state);
        }
    }

    Item {
        id: errorWidget
        anchors.fill: parent
        PlasmaComponents.Label {
            id: errorLabel
            anchors { top: parent.top; left: parent.left; right: parent.right; }
            text: errorMessage(owncloudSettings.error)

        }
        PlasmaComponents.ToolButton {
            id: startDaemonButton
            text: i18n("Start Sync Daemon")
            iconSource: "system-run"
            visible: owncloudSettings.error == OwncloudSettings.NoDaemonError
            anchors { top: errorLabel.bottom; left: parent.horizontalCenter; }

            onClicked: owncloudSettings.startDaemon()
        }
    }

    Item {
        id: setupNavigation
        anchors { left: parent.left; bottom: parent.bottom; bottomMargin: 24; }
        width: parent.width > maxWidth ? maxWidth : parent.width

        PlasmaComponents.ToolButton {
            id: navLogin
            text: i18n("Sign in")
            iconSource: "dialog-password"
            opacity: setupWizard.state == "login" ? 0 : 1
            Behavior on opacity { FadeAnimation { } }
            onClicked: setupWizard.state = "login"
            anchors { verticalCenter: parent.verticalCenter; left: parent.left; }
        }
        PlasmaComponents.ToolButton {
            id: navFavs
            text: i18n("Setup folders")
            iconSource: "folder-sync"
            opacity: ((setupWizard.state == "favorites") ||
                      (owncloudSettings.owncloudStatus != OwncloudSettings.Connected)) ? 0 : 1
            Behavior on opacity { FadeAnimation { } }
            onClicked: setupWizard.state = "favorites"
            anchors { verticalCenter: parent.verticalCenter; right: parent.right; }
        }
        PlasmaComponents.ToolButton {
            id: done
            text: i18n("Finish")
            iconSource: "dialog-ok-apply"
            opacity: (setupWizard.state == "favorites") ? 1 : 0
            Behavior on opacity { FadeAnimation { } }
            onClicked: {
                favoritesWidget.apply();
                owncloudModule.state = "default";
            }
            anchors { verticalCenter: parent.verticalCenter; right: parent.right; }
        }
    }

    LoginWidget {
        id: loginWidget
        anchors { top: parent.top; left: parent.left; bottom: parent.bottom; }
        width: parent.width > maxWidth ? maxWidth : parent.width
    }

    FavoritesWidget {
        id: favoritesWidget
        anchors { top: parent.top; left: parent.left; bottom: parent.bottom; rightMargin: 12; }
        width: parent.width > maxWidth ? maxWidth : parent.width

    }

    function errorMessage(e) {
        if (e == OwncloudSettings.NoError)
            return i18n("You are logged in.");
        if (e == OwncloudSettings.AuthenticationError)
            return i18n("The supplied username or password is wrong.");
        if (e == OwncloudSettings.NetworkError)
            return i18n("Cannot connect to ownCloud server.");
        if (e == OwncloudSettings.NoConfigurationError)
            return i18n("No ownCloud server configured.");
        if (e == OwncloudSettings.NoDaemonError)
            return i18n("The synchronization daemon is not running.");
        if (e == OwncloudSettings.CustomError)
            return i18n("Something unexpected happened.");
    }

    Behavior on opacity { FadeAnimation { } }

    Component.onCompleted: print("Errorhandler loaded.");
}
