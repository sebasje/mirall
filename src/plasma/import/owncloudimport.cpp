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
//#include <QScriptEngine>


#include <QDebug>

// void CoreBindingsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
// {
//     QQmlExtensionPlugin::initializeEngine(engine, uri);
//
//     QQmlContext *context = engine->rootContext();
//
//     Plasma::QuickTheme *theme = new Plasma::QuickTheme(engine);
//     context->setContextProperty("theme", theme);
//
//     Units *units = new Units(context);
//     context->setContextProperty("units", units);
//
//     if (!engine->rootContext()->contextObject()) {
//         KDeclarative::KDeclarative kdeclarative;
//         kdeclarative.setDeclarativeEngine(engine);
//         kdeclarative.setupBindings();
//     }
// }
//
void OwncloudImport::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.owncloud"));
    qDebug() << "OwncloudImport plugin created:)";
    //qmlRegisterType<OwncloudSettings>();
    qmlRegisterType<DirectoryLister>("org.kde.plasma.owncloud", 0, 1, "DirectoryLister");
    qmlRegisterType<DirModel>("org.kde.plasma.owncloud", 0, 1, "DirModel");
    qmlRegisterType<OwncloudSettings>("org.kde.plasma.owncloud", 0, 1, "OwncloudSettings");
    qmlRegisterType<OwncloudFolder>("org.kde.plasma.owncloud", 0, 1, "OwncloudFolder");
    qmlRegisterUncreatableType<SyncProgress>("org.kde.plasma.owncloud", 0, 1, "SyncProgress", "Access this type through owncloudSettings.progress");
    qmlRegisterType<MiniJob>("org.kde.plasma.owncloud", 0, 1, "MiniJob");
    qmlRegisterType<CreateFolderJob>("org.kde.plasma.owncloud", 0, 1, "CreateFolderJob");

/*
    qmlRegisterUncreatableType<Plasma::Types>(uri, 2, 0, "Types", "");
    qmlRegisterUncreatableType<Units>(uri, 2, 0, "Units", "");

    qmlRegisterType<Plasma::Svg>(uri, 2, 0, "Svg");
    qmlRegisterType<Plasma::FrameSvg>(uri, 2, 0, "FrameSvg");
    qmlRegisterType<Plasma::SvgItem>(uri, 2, 0, "SvgItem");
    qmlRegisterType<Plasma::FrameSvgItem>(uri, 2, 0, "FrameSvgItem");

    //qmlRegisterType<ThemeProxy>(uri, 2, 0, "Theme");
    qmlRegisterUncreatableType<Plasma::QuickTheme>(uri, 2, 0, "Theme", "It is not possible to instantiate Theme directly.");
    qmlRegisterType<ColorScope>(uri, 2, 0, "ColorScope");

    qmlRegisterType<Plasma::DataSource>(uri, 2, 0, "DataSource");
    qmlRegisterType<Plasma::DataModel>(uri, 2, 0, "DataModel");
    qmlRegisterType<Plasma::SortFilterModel, 0>(uri, 2, 0, "SortFilterModel");
    qmlRegisterType<Plasma::SortFilterModel, 1>(uri, 2, 1, "SortFilterModel");

    qmlRegisterType<PlasmaQuick::Dialog>(uri, 2, 0, "Dialog");
    qmlRegisterType<ToolTip>(uri, 2, 0, "ToolTipArea");

    qmlRegisterInterface<Plasma::Service>("Service");
    qRegisterMetaType<Plasma::Service *>("Service");
    qmlRegisterInterface<Plasma::ServiceJob>("ServiceJob");
    qRegisterMetaType<Plasma::ServiceJob *>("ServiceJob");
    qmlRegisterType<ServiceOperationStatus>(uri, 2, 0, "ServiceOperationStatus");
    qmlRegisterType<QAbstractItemModel>();

    qmlRegisterType<QQmlPropertyMap>();
    qmlRegisterType<IconItem>(uri, 2, 0, "IconItem");

    qmlRegisterInterface<Plasma::DataSource>("DataSource");
    qRegisterMetaType<Plasma::DataSource *>("DataSource");

    qmlRegisterType<Plasma::WindowThumbnail>(uri, 2, 0, "WindowThumbnail");*/
}


