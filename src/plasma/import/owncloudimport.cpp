/***************************************************************************
 *                                                                         *
 *   Copyright 2012,2015 Sebastian Kügler <sebas@kde.org>                  *
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
 *                                                                         *
 ***************************************************************************/

#include "owncloudimport.h"

//#include "owncloudsettingsplugin.h"
#include "owncloudsettings.h"
#include "owncloudfolder.h"
#include "directorylister.h"
#include "dirmodel.h"
#include "minijob.h"
#include "createfolderjob.h"

#include <QQmlContext>

#include <QDebug>

void OwncloudImport::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.owncloud"));
    qDebug() << "OwncloudImport plugin created:)";
    //qmlRegisterType<OwncloudSettings>();
    qmlRegisterType<DirectoryLister>("org.kde.plasma.owncloud", 1, 0, "DirectoryLister");
    qmlRegisterType<DirModel>("org.kde.plasma.owncloud", 1, 0, "DirModel");
    qmlRegisterType<OwncloudSettings>("org.kde.plasma.owncloud", 1, 0, "OwncloudSettings");
    qmlRegisterType<OwncloudFolder>("org.kde.plasma.owncloud", 1, 0, "OwncloudFolder");
    qmlRegisterUncreatableType<SyncProgress>("org.kde.plasma.owncloud", 1, 0, "SyncProgress", "Access this type through owncloudSettings.progress");
    qmlRegisterType<MiniJob>("org.kde.plasma.owncloud", 1, 0, "MiniJob");
    qmlRegisterType<CreateFolderJob>("org.kde.plasma.owncloud", 1, 0, "CreateFolderJob");

}


