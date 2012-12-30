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


#include "kfilesync.h"
#include <kdebug.h>
#include <KIO/AccessManager>

class KFileSyncPrivate {
public:
};

KFileSync::KFileSync(QObject *parent)
    : OwncloudSync(parent)
{
    d = new KFileSyncPrivate;

//     KIO::AccessManager *nam = new KIO::AccessManager(this);
//     ocInfo()->setNetworkAccessManager(nam);
    if(ocInfo()->isConfigured() ) {
        kDebug() << "OCInfo is configured, checkInstallation(), loadFolders()";

        ocInfo()->checkInstallation();
        loadFolders();
    }

}

KFileSync::~KFileSync()
{
    delete d;
}


#include "kfilesync.moc"
