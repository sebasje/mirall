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
#include "owncloudfolder.h"

#include "../applet/owncloud_interface.h"

#include <kdebug.h>
#include <QVariant>

#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeItem>

class OwncloudSettingsPrivate {
public:
    OwncloudSettings *q;
    OrgKdeOwncloudsyncInterface* client;
    QString name;
    QString localPath;
    QString remotePath;
    int status;
    int error;
    QString errorMessage;
    QString statusMessage;

    QList<OwncloudFolder*> folders;
    QVariantMap owncloudInfo;

    void loadFolders();
};

OwncloudSettings::OwncloudSettings(QObject* parent) :
    QObject(parent)
{
    d = new OwncloudSettingsPrivate;
    d->q = this;
    d->client = 0;

    d->error = OwncloudSettings::NoError;
    d->status = OwncloudSettings::Disconnected;

    kDebug() << "OwncloudSettings module loaded.";
    init();
    d->loadFolders();
    emit foldersChanged();

}

OwncloudSettings::~OwncloudSettings()
{
    kDebug() << "========================== owncloudsettings destroy";
    delete d;
}


void OwncloudSettings::init()
{
    if (!QDBusConnection::sessionBus().interface()->isServiceRegistered(QLatin1String("org.kde.owncloudsync"))) {
        setOwncloudStatus(OwncloudSettings::Error);
        setError(OwncloudSettings::NoDaemonError);
        delete d->client;
        d->client = 0; // invalidate client when dbus connection is lost
        return;
    } else if (d->client == 0) {

        d->client = new OrgKdeOwncloudsyncInterface("org.kde.owncloudsync", "/", QDBusConnection::sessionBus(), this);
        QObject::connect(d->client, SIGNAL(displayChanged(QString)), this, SLOT(setStatusMessage(QString)));
        QObject::connect(d->client, SIGNAL(statusMessageChanged(QString)), this, SLOT(setStatusMessage(QString)));
        QObject::connect(d->client, SIGNAL(statusChanged(int)), this, SLOT(setOwncloudStatus(int)));
        QObject::connect(d->client, SIGNAL(errorChanged(int)), this, SLOT(setError(int)));
        QObject::connect(d->client, SIGNAL(errorMessageChanged(QString)), this, SLOT(setErrorMessage(QString)));
        QObject::connect(d->client, SIGNAL(folderListChanged(const QVariantMap&)), this, SLOT(setFolderList(const QVariantMap&)));
        QObject::connect(d->client, SIGNAL(folderChanged(const QVariantMap&)), this, SLOT(setFolder(const QVariantMap&)));
        QObject::connect(d->client, SIGNAL(owncloudChanged(const QVariantMap&)), this, SLOT(setOwncloud(const QVariantMap&)));

        kDebug() << d->client->display();
        setStatusMessage(d->client->display());
        //refresh();
    }
}

void OwncloudSettings::setDisplay(const QString& n)
{
    kDebug() << "displayChanged" << n;
}

void OwncloudSettings::setStatusMessage(const QString& n)
{
    if (d->statusMessage != n) {
        kDebug() << "Setting status message: " << n;
        d->statusMessage = n;
        emit statusMessageChanged();
    }
}

QString OwncloudSettings::statusMessage() const
{
    return d->statusMessage;
}


// -- owncloud Info handling

void OwncloudSettings::setOwncloud(const QVariantMap& m)
{
    d->owncloudInfo = m;
    emit editionChanged();
    emit versionChanged();
    emit urlChanged();
}

QString OwncloudSettings::edition()
{
    return d->owncloudInfo["edition"].toString();
}

QString OwncloudSettings::version()
{
    return d->owncloudInfo["version"].toString();
}

void OwncloudSettings::setUrl(const QString& u)
{
    kWarning() << "setUrl is not supported yet.";
}

QString OwncloudSettings::url()
{
    return d->owncloudInfo["url"].toString();

}

int OwncloudSettings::owncloudStatus() const
{
    return d->status;
}

void OwncloudSettings::setOwncloudStatus(int i)
{
    if (d->status != i) {
        d->status = i;
        emit owncloudStatusChanged();
    }
}

int OwncloudSettings::error() const
{
    return d->error;
}

void OwncloudSettings::setError(int i)
{
    if (d->error != i) {
        d->error = i;
        emit errorChanged();
    }
}

QString OwncloudSettings::errorMessage() const
{
    return d->errorMessage;
}

void OwncloudSettings::setErrorMessage(const QString& n)
{
    if (d->errorMessage != n) {
        d->errorMessage = n;
        emit errorMessageChanged();
    }
}

void OwncloudSettings::startDaemon()
{
    // start daemon
    //d->client->startDaemon(); FIXME
    kDebug() << "Start Daemon...";
    init();
}

// -- Folder handling

void OwncloudSettings::setFolderList(const QVariantMap& m)
{
    //kDebug() << "Folder changed!" << m;
    foreach (QObject* f, d->folders) {
        delete f;
    }
    d->folders.clear();
    foreach (const QString &k, m.keys()) {
        OwncloudFolder *f1;

        f1 = new OwncloudFolder(this);
        f1->setDisplayName(k);
        f1->setFolderStatus(m[k].toInt());
        d->folders << f1;
    }
    emit foldersChanged();
}

void OwncloudSettings::setFolder(const QVariantMap& m)
{
    QString alias = m["name"].toString();
    OwncloudFolder *folder = 0;

    bool exists = false;
    foreach (OwncloudFolder *f, d->folders) {

        if (f->displayName() == alias) {
            folder = f;
            exists = true;
            continue;
        }
    }
    if (exists) {
        //kDebug() << "OC Updating existing folder" << alias;
    } else {
        kDebug() << "OC New Folder" << alias;
        folder = new OwncloudFolder(this);
        d->folders << folder;
        connect(folder, SIGNAL(enableFolder(const QString&, bool)), this, SLOT(enableFolder(const QString&, bool)));
    }
    folder->setDisplayName(alias);
    folder->setFolderStatus(m["status"].toInt());
    folder->setErrorMessage(m["errorMessage"].toString());
    //kDebug() << "OC Updating" << alias << folder->folderStatus() << folder->errorMessage();

    emit foldersChanged();
}

void OwncloudSettings::enableFolder(const QString& name, bool enabled)
{
    if (d->client) {
        d->client->enableFolder(name, enabled);
    }
}

QDeclarativeListProperty<OwncloudFolder> OwncloudSettings::folders()
{
    return QDeclarativeListProperty<OwncloudFolder>(this, d->folders);
}

void OwncloudSettings::refresh()
{
    if (d->client) {
        d->client->refresh();
    }
}

void OwncloudSettingsPrivate::loadFolders()
{
    return;
    OwncloudFolder *f1;

    f1 = new OwncloudFolder(q);
    f1->setDisplayName("My Computer");
    folders << f1;

    OwncloudFolder *f;

    f = new OwncloudFolder(q);
    f->setDisplayName("My Documents");
    f->setFolderStatus(OwncloudFolder::Error);
    folders << f;

    f = new OwncloudFolder(q);
    f->setDisplayName("My Videos");
    folders << f;

    f = new OwncloudFolder(q);
    f->setDisplayName("My Images");
    f->setFolderStatus(OwncloudFolder::Waiting);
    folders << f;

    f = new OwncloudFolder(q);
    f->setDisplayName("My Files");
    f->setFolderStatus(OwncloudFolder::Disabled);
    folders << f;

    kDebug() << "Loaded folders : " << folders.count();
}


#include "owncloudsettings.moc"
