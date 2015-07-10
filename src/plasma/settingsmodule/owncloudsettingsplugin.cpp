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

#include "owncloudsettingsplugin.h"
#include "owncloudsettings.h"
#include "owncloudfolder.h"
#include "directorylister.h"
#include "dirmodel.h"
#include "minijob.h"
#include "createfolderjob.h"

#include <QDebug>
#include <KIcon>

#include <QVariant>

#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KSharedConfig>
#include <KStandardDirs>
#include <KConfigGroup>
#include <KGlobalSettings>

#include <QtDeclarative/qdeclarative.h>
#include <QDeclarativeItem>
#include <QDate>

K_PLUGIN_FACTORY(OwncloudSettingsFactory, registerPlugin<OwncloudSettingsPlugin>();)
K_EXPORT_PLUGIN(OwncloudSettingsFactory("active_settings_owncloud"))

OwncloudSettingsPlugin::OwncloudSettingsPlugin(QObject *parent, const QVariantList &list)
    : QObject(parent)
{
    Q_UNUSED(list)

    qDebug() << "OwncloudSettingsPlugin created:)";
    //qmlRegisterType<OwncloudSettings>();
    qmlRegisterType<DirectoryLister>("org.kde.plasma.owncloud", 0, 1, "DirectoryLister");
    qmlRegisterType<DirModel>("org.kde.plasma.owncloud", 0, 1, "DirModel");
    qmlRegisterType<OwncloudSettings>("org.kde.plasma.owncloud", 0, 1, "OwncloudSettings");
    qmlRegisterType<OwncloudFolder>("org.kde.plasma.owncloud", 0, 1, "OwncloudFolder");
    qmlRegisterUncreatableType<SyncProgress>("org.kde.plasma.owncloud", 0, 1, "SyncProgress", "Access this type through owncloudSettings.progress");
    qmlRegisterType<MiniJob>("org.kde.plasma.owncloud", 0, 1, "MiniJob");
    qmlRegisterType<CreateFolderJob>("org.kde.plasma.owncloud", 0, 1, "CreateFolderJob");
}

OwncloudSettingsPlugin::~OwncloudSettingsPlugin()
{
    qDebug() << "oc plugin del'ed";
}


#include "owncloudsettingsplugin.moc"
