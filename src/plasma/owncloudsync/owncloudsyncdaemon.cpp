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

#include "owncloudsyncdaemon.h"
#include "kfilesync.h"

#include "owncloudsyncadaptor.h"

#include <QtDBus/QDBusConnection>


OwncloudSyncDaemon* OwncloudSyncDaemon::s_self = 0;

OwncloudSyncDaemon::OwncloudSyncDaemon( QObject* parent )
    : QObject( parent ),
      m_currentState(StateDisabled)
{
    s_self = this;

    init();
}


OwncloudSyncDaemon::~OwncloudSyncDaemon()
{
    QDBusConnection::sessionBus().unregisterService( "org.kde.owncloudsync" );
}


void OwncloudSyncDaemon::init()
{
    OwncloudSync* syncdaemon = new KFileSync(this);
    //syncdaemon->init();
    //OwncloudSync* syncdaemon = new OwncloudSync(this);
    new OwncloudsyncAdaptor(syncdaemon);

    QDBusConnection::sessionBus().registerService( "org.kde.owncloudsync" );

    QDBusConnection::sessionBus().registerObject( "/", syncdaemon);
}

void OwncloudSyncDaemon::quit()
{
    QCoreApplication::instance()->quit();
}

OwncloudSyncDaemon* OwncloudSyncDaemon::self()
{
    return s_self;
}

#include "owncloudsyncdaemon.moc"
