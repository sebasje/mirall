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

#include "owncloudsync.h"
#include "owncloudsyncdaemon.h"

#include "owncloudsyncadaptor.h"

#include <KDebug>
#include <KGlobal>
#include <KStandardDirs>

#include <QtDBus/QDBusConnection>


OwncloudSync* OwncloudSync::s_self = 0;

OwncloudSync::OwncloudSync( QObject* parent )
    : QObject( parent ),
      m_currentState(StateDisabled)
{
    s_self = this;

    OwncloudSyncDaemon* syncdaemon = new OwncloudSyncDaemon(this);
    new OwncloudsyncAdaptor(syncdaemon);

    QDBusConnection::sessionBus().registerService( "org.kde.owncloudsync" );

    QDBusConnection::sessionBus().registerObject( "/", syncdaemon);
    init();
}


OwncloudSync::~OwncloudSync()
{
    QDBusConnection::sessionBus().unregisterService( "org.kde.owncloudsync" );
}


void OwncloudSync::init()
{
}

void OwncloudSync::quit()
{
    QCoreApplication::instance()->quit();
}

OwncloudSync* OwncloudSync::self()
{
    return s_self;
}

#include "owncloudsync.moc"
