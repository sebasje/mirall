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

#include <qdebug.h>
#include <QVariant>


class OwncloudSyncDaemonPrivate {
public:
    OwncloudSyncDaemon *q;
    QString name;
    QString localPath;
    QString remotePath;
    QString display;

    void loadFolders();
};

OwncloudSyncDaemon::OwncloudSyncDaemon(QObject *parent)
{
    d = new OwncloudSyncDaemonPrivate;
    d->q = this;
    d->display = "ownCloud Sync Daemon";
    qDebug() << "OwncloudSyncDaemon loaded.";

}

OwncloudSyncDaemon::~OwncloudSyncDaemon()
{
    qDebug() << "===== owncloudsyncdaemon destroy";
    delete d;
}

QString OwncloudSyncDaemon::display()
{
    qDebug() << "display() " << d->display;
    return d->display;
}


void OwncloudSyncDaemonPrivate::loadFolders()
{
    //kDebug() << "Loaded folders : " << folders.count();
}


#include "owncloudsyncdaemon.moc"
