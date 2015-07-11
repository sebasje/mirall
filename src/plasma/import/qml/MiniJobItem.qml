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
import org.kde.plasma.owncloud 1.0

Item {
    id: jobItem
    //width: 200
    height: 64

    property QtObject currentJob
    property bool running: (progressBar.value != 0 && progressBar.value != 100)

    Row {
        anchors.margins: 12
        spacing: 12

        PlasmaComponents.ProgressBar {
            id: progressBar
            width: 200
            height: 20
            minimumValue: 0
            maximumValue: 100
            //value: 20
        }
        PlasmaComponents.Button {
            text: !running ? "Start" : "Stop"
            onClicked: {
                if (!running) {
                    start();
                } else {
                    currentJob.stop();
                    running = false;
                    blabla.text = "Job stopped.";
                    //progressBar.value = 0;
                }
            }
        }
        PlasmaComponents.Label {
            id: blabla
            //width: parent.width/3
        }
    }


    function start() {
        blabla.text = "Starting Job...";
        currentJob = owncloudSettings.createMiniJob();
        currentJob.finished.connect(jobFinished);
        currentJob.advanceChanged.connect(updateProgress);

        progressBar.value = currentJob.advance;
        currentJob.start();
        running = true;
        /*
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
        */
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
        running = false;
    }


    Component.onCompleted:  {
        print("ExampleJobItem init'ed");
        //start();
    }
}