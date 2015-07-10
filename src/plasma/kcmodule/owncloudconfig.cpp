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

// Own
#include "owncloudconfig.h"

// from lib/
#include "owncloudsettings.h"
#include "owncloudfolder.h"
#include "directorylister.h"
#include "dirmodel.h"
#include "minijob.h"
#include "createfolderjob.h"
#include "syncprogress.h"

// Qt
#include <QBoxLayout>
#include <QDeclarativeView>

// KDE
#include <kconfiggroup.h>
#include <QDebug>
#include <kstandarddirs.h>
#include <Plasma/PackageStructure>
#include <Plasma/Package>
#include <kdeclarative.h>


#include "kcmowncloud.h"
//#include "ui_owncloudconfig.h"


class OwncloudConfigPrivate {
public:
    KDeclarative kdeclarative;
    Plasma::PackageStructure::Ptr structure;
    Plasma::Package *package;
    QString packageName;

    QDeclarativeView *declarativeView;

};


//-----------------------------------------------------------------------------

OwncloudConfig::OwncloudConfig(QWidget *parent, const QVariantList &)
    : KCModule( KcmOwncloud::componentData(), parent )
{

    //setButtons(0);

    d = new OwncloudConfigPrivate;
    d->declarativeView = new QDeclarativeView(this);

    d->kdeclarative.setDeclarativeEngine(d->declarativeView->engine());
    d->kdeclarative.initialize();
    //binds things like kconfig and icons
    d->kdeclarative.setupBindings();
    d->declarativeView->setStyleSheet(QString("background: transparent"));
    d->declarativeView->setResizeMode(QDeclarativeView::SizeRootObjectToView);

//     // avoid flicker on show
    //d->declarativeView->setAttribute(Qt::WA_OpaquePaintEvent);
    //d->declarativeView->setAttribute(Qt::WA_NoSystemBackground);
    //d->declarativeView->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    //d->declarativeView->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    QBoxLayout *lay = new QBoxLayout(QBoxLayout::TopToBottom, this);
    lay->addWidget(d->declarativeView);
    lay->setMargin(0);

    setQuickHelp( i18n("<h1>Owncloud</h1>\n"
    "This module allows you to configure ownCloud servers and folders you want to synchronize."));

    qmlRegisterType<DirectoryLister>("org.kde.plasma.owncloud", 0, 1, "DirectoryLister");
    qmlRegisterType<DirModel>("org.kde.plasma.owncloud", 0, 1, "DirModel");
    qmlRegisterType<OwncloudSettings>("org.kde.plasma.owncloud", 0, 1, "OwncloudSettings");
    qmlRegisterType<OwncloudFolder>("org.kde.plasma.owncloud", 0, 1, "OwncloudFolder");
    qmlRegisterType<MiniJob>("org.kde.plasma.owncloud", 0, 1, "MiniJob");
    qmlRegisterType<CreateFolderJob>("org.kde.plasma.owncloud", 0, 1, "CreateFolderJob");
    qmlRegisterUncreatableType<SyncProgress>("org.kde.plasma.owncloud", 0, 1, "SyncProgress", "Access this type through owncloudSettings.progress");


    Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.active.settings.owncloud", structure);
    const QString qmlFile = package->filePath("ui", "OwncloudSettingsModule.qml");
    delete package;
    d->declarativeView->setSource(qmlFile);
}

void OwncloudConfig::load()
{
    qDebug() << "Load settings....";
    emit changed(true);
}

void OwncloudConfig::defaults()
{
    qDebug() << "loading defaults....";

}


void OwncloudConfig::save()
{
    qDebug() << "Save settings....";
}


#include "owncloudconfig.moc"
