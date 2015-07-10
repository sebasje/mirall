/***************************************************************************
 *   Copyright 2012 by Sebastian Kügler <sebas@kde.org>                    *
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

#include "owncloudapplet.h"
#include "owncloudsettings.h"
#include "owncloudfolder.h"
#include "directorylister.h"
#include "dirmodel.h"
#include "createfolderjob.h"
#include "syncprogress.h"

#include <QtDeclarative>
//#include <QQuickItem>

#include <QDebug>
#include <KConfigDialog>
#include <KConfigGroup>

#include <Plasma/DeclarativeWidget>
#include <Plasma/Package>

OwncloudApplet::OwncloudApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_declarativeWidget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
}

OwncloudApplet::~OwncloudApplet()
{
}

void OwncloudApplet::init()
{
    setPopupIcon("owncloud");
    configChanged();

    Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.active.settings.owncloud", structure);
    const QString qmlFile = package->filePath("ui", "OwncloudPlasmoid.qml");
    delete package;

    //qDebug() << " Loading QML File from package: " << qmlFile;
    m_declarativeWidget = new Plasma::DeclarativeWidget(this);

    connect(m_declarativeWidget->mainComponent(), SIGNAL(statusChanged(QQmlComponent::Status)), SLOT(connectObjects(QQmlComponent::Status)));

    qmlRegisterType<DirectoryLister>("org.kde.plasma.owncloud", 0, 1, "DirectoryLister");
    qmlRegisterType<DirModel>("org.kde.plasma.owncloud", 0, 1, "DirModel");
    qmlRegisterType<OwncloudSettings>("org.kde.plasma.owncloud", 0, 1, "OwncloudSettings");
    qmlRegisterType<OwncloudFolder>("org.kde.plasma.owncloud", 0, 1, "OwncloudFolder");
    qmlRegisterType<CreateFolderJob>("org.kde.plasma.owncloud", 0, 1, "CreateFolderJob");
    qmlRegisterUncreatableType<SyncProgress>("org.kde.plasma.owncloud", 0, 1, "SyncProgress", "Access this type through owncloudSettings.progress");

    m_declarativeWidget->setQmlPath(qmlFile);
    m_declarativeWidget->setMinimumSize(220, 250);
    QTimer::singleShot(1000, this, SLOT(connectObjects()));
}

void OwncloudApplet::connectObjects()
{
    connectObjects(QQmlComponent::Ready);
}

void OwncloudApplet::connectObjects(QQmlComponent::Status s)
{
    qDebug() << "CONNECT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    if (s != QQmlComponent::Ready) {
        return;
    }
    qDebug() << "looking for owncloudModule component ...";
    // "owncloudSettings" is defined as objectName in the QML file
    OwncloudSettings* ocSettings = m_declarativeWidget->rootObject()->findChild<OwncloudSettings*>("owncloudSettings");
    if (ocSettings) {
        connect(ocSettings, SIGNAL(updateGlobalStatus(int)),
                this, SLOT(updatePopupIcon(int)));
        qDebug() << "Connnected....: )";
    } else {
        kError() << "owncloudModule component not found.";
    }
}

void OwncloudApplet::updatePopupIcon(int e)
{
    QString icon;
    if (e == OwncloudFolder::Running) {
        setBusy(true);
    } else {
        setBusy(false);
    }
    if (e == OwncloudFolder::Idle) icon = "owncloud";
    if (e == OwncloudFolder::Waiting) icon = "task-ongoing";
    if (e == OwncloudFolder::Running) icon = "owncloud";
    if (e == OwncloudFolder::Disabled) icon = "user-offline";
    if (e == OwncloudFolder::Error) icon = "task-attempt";

    //qDebug() << "===========> Setting popup icon: " << icon;
    setPopupIcon(icon);
}

void OwncloudApplet::configChanged()
{
    // ...
}

QQuickItem *OwncloudApplet::graphicsWidget()
{
    return m_declarativeWidget;
}

void OwncloudApplet::configAccepted()
{
}

void OwncloudApplet::createConfigurationInterface(KConfigDialog *parent)
{
    parent->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

#include "owncloudapplet.moc"
