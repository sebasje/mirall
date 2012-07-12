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
#include "owncloudfolder.h"

#include <qdebug.h>
#include <QVariant>
#include <QTimer>


class OwncloudSyncDaemonPrivate {
public:
    OwncloudSyncDaemon *q;
    QString name;
    QString localPath;
    QString remotePath;
    QString display;

    QVariantMap folderList;
    QHash<QString, QVariantMap> folders;
    QTimer *timer;

    void loadFolders();

    int c;
};

OwncloudSyncDaemon::OwncloudSyncDaemon(QObject *parent)
{
    d = new OwncloudSyncDaemonPrivate;
    d->q = this;
    d->loadFolders();
    d->display = "ownCloud Sync Daemon";

    d->timer = new QTimer(this);
    d->c = 0;
    d->timer->setInterval(3000);
    d->timer->start();
    connect(d->timer, SIGNAL(timeout()), SLOT(timeout()));
    emit folderListChanged(d->folderList);
    qDebug() << "OwncloudSyncDaemon loaded.";
}

OwncloudSyncDaemon::~OwncloudSyncDaemon()
{
    qDebug() << "===== owncloudsyncdaemon destroy";
    delete d;
}

void OwncloudSyncDaemon::timeout()
{
    //qDebug() << "timer " << d->c;
    emit statusMessageChanged(QString("Timeout has fired " + QString::number(d->c/4) + " times."));
    d->c++;

    d->loadFolders();
    emit folderListChanged(d->folderList);
    //qDebug() << d->folderList;
}


QString OwncloudSyncDaemon::display()
{
    qDebug() << "display() " << d->display;
    return d->display;
}

QVariantMap OwncloudSyncDaemon::folder(QString name)
{
    qDebug() << "folder()" << name;
    return d->folders[name];
}

QVariantMap OwncloudSyncDaemon::folderList()
{
    return d->folderList;
}

void OwncloudSyncDaemonPrivate::loadFolders()
{
    //kDebug() << "Loaded folders : " << folders.count();
    QStringList fs;
    fs << "Nirvana" << "Pearl Jam" << "Lana del Rey" << "Placebo" << "Cash";
    foreach (const QString &_f, fs) {
        int s = 0;
        int r = c % 4;
        //qDebug() << " c" << c << " r" << r;
        if (r == 0) s = OwncloudFolder::Idle;
        if (r == 1) s = OwncloudFolder::Disabled;
        if (r == 2) s = OwncloudFolder::Waiting;
        if (r == 3) s = OwncloudFolder::Error;

        folderList[_f] = s;

        QVariantMap m;
        m["name"] = _f;
        m["localPath"] = "/home/sebas/" + _f;
        folders[name] = m;
        c++;
    }
}


#include "owncloudsyncdaemon.moc"
