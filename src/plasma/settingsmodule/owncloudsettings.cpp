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


#include "owncloudsettings.h"

#include <kdebug.h>
// #include <KIcon>
// #include <KLocale>

// #include <QStandardItemModel>
// #include <QTimer>
#include <QVariant>

// #include <kdemacros.h>
// #include <KPluginFactory>
// #include <KPluginLoader>
// #include <KSharedConfig>
// #include <KStandardDirs>
// #include <KConfigGroup>
// #include <KGlobalSettings>
// #include <KSystemTimeZone>
// #include <KTimeZone>

#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeItem>
#include <QtCore/QDate>


class OwncloudSettingsPrivate {
public:
    OwncloudSettings *q;
    QString name;
    QString localPath;
    QString remotePath;

    QList<QObject*> folders;
};

OwncloudSettings::OwncloudSettings()
{
    d = new OwncloudSettingsPrivate;
    d->q = this;

    kDebug() << "OwncloudSettings module loaded.";
}

OwncloudSettings::~OwncloudSettings()
{
    kDebug() << "========================== owncloudsettings destroy";
    delete d;
}


#include "owncloudsettings.moc"
