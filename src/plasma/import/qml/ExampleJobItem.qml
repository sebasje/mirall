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
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

Item {
    id: jobItem
    width: 200
    height: 200

    property Item currentJob

    Column {
        anchors.margins: 12
        spacing: 12

        PlasmaComponents.Label {
            id: blabla
        }
        PlasmaComponents.ProgressBar {
            id: progressBar
            width: parent.width
            height: 20
            minimumValue: 0
            maximumValue: 100
            value: 20
        }
        PlasmaComponents.Button {

            text: "Start"
            onClicked: start()
        }
    }

//     ExampleJob {
//
//         onAdvanceChanged: {
//             print("pc: " + advance);
//             progressBar.value = advance;
//         }
//     }

    function start() {
        blabla.text = "Starting Job...";

        var myjob = {};
        var myjobComponent = Qt.createComponent("ExampleJob.qml");
        myjob = myjobComponent.createObject(jobItem);

        print("TT: " + typeof(myjob));
        myjob.finished.connect(jobFinished);
        myjob.advanceChanged.connect(updateProgress);
        //progressBar.value = myjob.advance;
        //progressBar.value = myjob.advance;
        myjob.run();
        jobItem.currentJob = myjob;
    }

    function updateProgress() {
        progressBar.value = currentJob.advance;
    }

    function jobFinished(success) {
        var b;
        if (success) {
            b = i18n("Job finished successfully.");
        } else {
            b = i18n("Job failed.");
        }
        print("finished: " + b);
        blabla.text = b;
    }


    Component.onCompleted:  {
        print("ExampleJobItem init'ed");
        start();
    }
}