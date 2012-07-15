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
    QString statusMessage;

    QList<OwncloudFolder*> folders;

    void loadFolders();
};

OwncloudSettings::OwncloudSettings()
{
    d = new OwncloudSettingsPrivate;
    d->q = this;

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
    d->client = new OrgKdeOwncloudsyncInterface("org.kde.owncloudsync", "/", QDBusConnection::sessionBus(), this);
    QObject::connect(d->client, SIGNAL(displayChanged(QString)), this, SLOT(setStatusMessage(QString)));
    QObject::connect(d->client, SIGNAL(statusMessageChanged(QString)), this, SLOT(setStatusMessage(QString)));
    QObject::connect(d->client, SIGNAL(folderListChanged(const QVariantMap&)), this, SLOT(setFolderList(const QVariantMap&)));
    QObject::connect(d->client, SIGNAL(folderChanged(const QVariantMap&)), this, SLOT(setFolder(const QVariantMap&)));

    kDebug() << d->client->display();
    setStatusMessage(d->client->display());
}

void OwncloudSettings::setDisplay(const QString& n)
{
    kDebug() << "displayChanged" << n;
}

void OwncloudSettings::setStatusMessage(const QString& n)
{
    if (d->statusMessage != n) {
        //kDebug() << "Setting status message: " << n;
        d->statusMessage = n;
        emit statusMessageChanged();
    }
}

QString OwncloudSettings::statusMessage()
{
    return d->statusMessage;
}

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
    }
    folder->setDisplayName(alias);
    folder->setFolderStatus(m["status"].toInt());
    folder->setErrorMessage(m["errorMessage"].toString());
    kDebug() << "OC Updating" << alias << folder->folderStatus() << folder->errorMessage();

    emit foldersChanged();
}

QDeclarativeListProperty<OwncloudFolder> OwncloudSettings::folders()
{
    return QDeclarativeListProperty<OwncloudFolder>(this, d->folders);
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
