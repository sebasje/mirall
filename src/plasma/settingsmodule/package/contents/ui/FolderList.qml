import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.plasma.owncloud 0.1 as OC

ListView {
    id: folderList
    header: PlasmaExtras.Heading { level: 3; text: i18n("My Folders") }
    model: owncloudSettings

    delegate: PlasmaComponents.Label {
        text: "FOlder: "
        width: 200
        height: 48
    }

    OC.OwncloudSettings {
        id: owncloudSettings
    }

}
