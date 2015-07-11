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
// import org.kde.plasma.components 2.0 as PlasmaComponents
// import org.kde.plasma.extras 2.0 as PlasmaExtras
// import org.kde.plasma.owncloud 1.0
// import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

Item {
    id: job

    property int advance: 80 // between 0 and 100
    signal finished(bool success)
    signal advanceChanged(int ad)

//     property int cnt: 0;
//     QtObject {
//         id: d
//         property int cnt: 0
//
    function run() {
        // do the actual work
        print("Run, Forest, run!");
        job.advance = 0;
        fakeTimer.start();
    }

    function finish() {
        print("Finishing ");
        job.finished(true);
        //cnt = 0;
    }

    Timer {
        id: fakeTimer
        interval: 100
        repeat: true
        onTriggered: {
            advance = advance + 4;
            print("." + advance);
            if (job.advance >= 100) {
                fakeTimer.stop();
                finish();
            }
            advanceChanged(advance);
        }
    }

    onAdvanceChanged: { print(advance); }

    Component.onCompleted: print("ExampleJob init'ed");
}