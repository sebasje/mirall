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

#include "owncloudfolder.h"

#include <kdebug.h>


class OwncloudFolderPrivate {
public:
    OwncloudFolder *q;
    QString name;
    QString localPath;
    QString remotePath;
    int error;
    QString errorMessage;
    int status;
    QDateTime syncTime;
};

OwncloudFolder::OwncloudFolder(QObject* parent)
    : QObject(parent)
{
    d = new OwncloudFolderPrivate;
    d->q = this;
    setName("d-fault.");
    setDisplayName(name());
    d->error = NoError;
    d->status = Idle;
}

OwncloudFolder::~OwncloudFolder()
{
    delete d;
}

int OwncloudFolder::folderStatus() const
{
    return d->status;
}

void OwncloudFolder::setFolderStatus(int i)
{
    if (d->status != i) {
        if (d->status == Running && i == Idle) {
            setSyncTime(QDateTime::currentDateTime());
        }
        d->status = i;
        emit folderStatusChanged();
    }
}

int OwncloudFolder::error() const
{
    return d->error;
}

void OwncloudFolder::setError(int i)
{
    if (d->error != i) {
        d->error = i;
        emit errorChanged();
    }
}

QString OwncloudFolder::errorMessage() const
{
    return d->errorMessage;
}

void OwncloudFolder::setErrorMessage(const QString& n)
{
    if (d->errorMessage != n) {
        d->errorMessage = n;
        emit errorMessageChanged();
    }
}

QString OwncloudFolder::name() const
{
    return d->name;
}

void OwncloudFolder::setName(const QString &n)
{
    d->name = n;
    emit nameChanged();
    setDisplayName(n);
}

QString OwncloudFolder::displayName() const
{
    return d->name;
}

void OwncloudFolder::setDisplayName(const QString &n)
{
    if (d->name == n) {
        return;
    }
    d->name = n;
    emit displayNameChanged();
}

QString OwncloudFolder::localPath() const
{
    return d->localPath;
}

void OwncloudFolder::setLocalPath(const QString& p)
{
    if (d->localPath != p) {
        d->localPath = p;
        emit localPathChanged();
    }
}

QString OwncloudFolder::remotePath() const
{
    return d->remotePath;
}

void OwncloudFolder::setRemotePath(const QString& p)
{
    if (d->remotePath != p) {
        d->remotePath = p;
        emit remotePathChanged();
    }
}

// Q_INVOKABLES

void OwncloudFolder::enable()
{
    kDebug() << "Enable folder." << d->name;
    //setFolderStatus(Idle);
    emit enableFolder(d->name, true);
}

void OwncloudFolder::disable()
{
    kDebug() << "Disable folder." << d->name;
    //setFolderStatus(Disabled);
    emit enableFolder(d->name, false);
}

void OwncloudFolder::remove()
{
    kDebug() << "Remove folder." << d->name;
}

QDateTime OwncloudFolder::syncTime() const
{
    return d->syncTime;
}

void OwncloudFolder::setSyncTime(const QDateTime& dt)
{
    if (dt != d->syncTime) {
        d->syncTime = dt;
        //qDebug() << " OC OwncloudFolder::sync time updated: " << dt;
        emit syncTimeChanged();
    }
}



#include "owncloudfolder.moc"
