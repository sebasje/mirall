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
 ***************************************************************************/

#include "owncloudsyncdaemon.h"

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kcomponentdata.h>
#include <QDebug>

#include <QtCore/QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>

#include <signal.h>


namespace {
    OwncloudSyncDaemon* s_server = 0;

#ifndef Q_OS_WIN
    void signalHandler( int signal )
    {
        switch( signal ) {
        case SIGHUP:
        case SIGQUIT:
        case SIGTERM:
        case SIGINT:
            if ( s_server ) {
                s_server->quit();
            }
        }
    }

    void installSignalHandler() {
        struct sigaction sa;
        ::memset( &sa, 0, sizeof( sa ) );
        sa.sa_handler = signalHandler;
        sigaction( SIGHUP, &sa, 0 );
        sigaction( SIGINT, &sa, 0 );
        sigaction( SIGQUIT, &sa, 0 );
        sigaction( SIGTERM, &sa, 0 );
    }
#endif
}

int main(int argc, char **argv)
{
//     KAboutData aboutData( "ownCloudSync", "owncloudsync",
//                           ki18n("ownCloud Sync client"),
//                           "0.1",
//                           ki18n("ownCloud Sync - Data Synchronization Daemon"),
//                           KAboutData::License_GPL,
//                           ki18n("Copyright 2012, Sebastian Kügler"),
//                           KLocalizedString(),
//                           "http://owncloud.org" );
//     aboutData.addAuthor(ki18n("Sebastian Kügler"),ki18n("Maintainer"), "sebas@kde.org");
//
//     KCmdLineArgs::init( argc, argv, &aboutData );
//     KComponentData componentData( &aboutData );
    QCoreApplication app(argc, argv);
    app.setApplicationName("Mirall"); // Needed to locate the right config file

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(QLatin1String("org.kde.owncloudsync"))) {
        fprintf( stderr, "owncloudsync already running.\n" );
        app.quit();
        return 0;
    }

#ifndef Q_OS_WIN
    installSignalHandler();
#endif

    s_server = new OwncloudSyncDaemon(&app);
    return app.exec();
}
