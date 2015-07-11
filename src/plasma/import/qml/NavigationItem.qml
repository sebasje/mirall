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
    id: navigationItem
    height: 48

    signal apply()


    //anchors { left: parent.left; bottom: parent.bottom; bottomMargin: 0; }
    //width: parent.width > maxWidth ? maxWidth : parent.width

    PlasmaComponents.ToolButton {
        id: navLogin
        text: i18n("Sign in")
        iconSource: "dialog-password"
        opacity: (root.state != "default" && root.state != "addFolder" && setupWizard.state != "login") ? 1 : 0
        Behavior on opacity { FadeAnimation { } }
        onClicked: {
            setupWizard.state = "login"
            root.state = "setup";
        }
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; }
    }
    PlasmaComponents.ToolButton {
        id: navFavs
        text: i18n("Settings")
        iconSource: "configure"
        //opacity: ((root.state != "addFolder") || (setupWizard.state == "favorites") || (owncloudSettings.owncloudStatus != OwncloudSettings.Connected)) ? 0 : 1
        opacity: root.state == "default" ? 1 : 0
        Behavior on opacity { FadeAnimation { } }
        onClicked: {
            setupWizard.state = "favorites";
            root.state = "setup";
        }
        anchors { verticalCenter: parent.verticalCenter; right: parent.right; }
    }
    PlasmaComponents.ToolButton {
        id: done
        text: i18n("OK")
        iconSource: "dialog-ok-apply"
        opacity: (root.state != "default")  && ((setupWizard.state == "favorites") && (root.state == "setup")) || ((setupWizard.state == "login" && owncloudSettings.owncloudStatus == OwncloudSettings.Connected))? 1 : 0
        Behavior on opacity { FadeAnimation { } }
        onClicked: {
            if (setupWizard.state == "favorites") {
                apply();
            }
            root.state = "default";
            setupWizard.state = "favorites";
        }
        anchors { verticalCenter: parent.verticalCenter; right: parent.right; }
    }
}
