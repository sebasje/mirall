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

#include <kdebug.h>
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
#include <QtDeclarative/QDeclarativeItem>
#include <QtCore/QDate>

K_PLUGIN_FACTORY(OwncloudSettingsFactory, registerPlugin<OwncloudSettingsPlugin>();)
K_EXPORT_PLUGIN(OwncloudSettingsFactory("active_settings_owncloud"))

OwncloudSettingsPlugin::OwncloudSettingsPlugin(QObject *parent, const QVariantList &list)
    : QObject(parent)
{
    Q_UNUSED(list)

    kDebug() << "OwncloudSettingsPlugin created:)";
    qmlRegisterType<OwncloudSettings>();
    qmlRegisterType<OwncloudSettings>("org.kde.active.settings", 0, 1, "OwncloudSettings");
}

OwncloudSettingsPlugin::~OwncloudSettingsPlugin()
{
    kDebug() << "oc plugin del'ed";
}


#include "owncloudsettingsplugin.moc"
