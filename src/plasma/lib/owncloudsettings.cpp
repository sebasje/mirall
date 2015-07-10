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
#include "minijob.h"
#include "createfolderjob.h"
#include "syncprogress.h"

#include <QDebug>
#include <KLocale>
#include <KProcess>
#include <KRun>
#include <KStandardDirs>

#include <QTimer>
#include <QVariant>

#include <QDBusServiceWatcher>
#include <QtDeclarative/qdeclarative.h>
#include <QDeclarativeItem>

// The DBus interface definition
#include "../applet/owncloud_interface.h"


class OwncloudSettingsPrivate {
public:
    OwncloudSettings *q;
    OrgKdeOwncloudsyncInterface* client;
    QString name;
    QString localPath;
    QString remotePath;
    int status; // OwncloudSettings::Status
    int globalStatus; // OwncloudFolder::Status
    int error;
    QHash<QString, CreateFolderJob*> createFolderJobs;

    QList<OwncloudFolder*> folders;
    QVariantMap owncloudInfo;
    SyncProgress *progress;
    QDBusServiceWatcher *serviceWatcher;
};

OwncloudSettings::OwncloudSettings(QObject* parent) :
    QObject(parent)
{
    d = new OwncloudSettingsPrivate;
    d->progress = new SyncProgress(this);
    d->q = this;
    d->client = 0;

    d->error = OwncloudSettings::NoError;
    d->status = OwncloudSettings::Disconnected;

    qDebug() << "OwncloudSettings module loaded.";

    d->serviceWatcher = new QDBusServiceWatcher("org.kde.owncloudsync",
                                                QDBusConnection::sessionBus(),
                                                QDBusServiceWatcher::WatchForRegistration & QDBusServiceWatcher::WatchForUnregistration,
                                                this);
    connect(d->serviceWatcher, SIGNAL(serviceRegistered(QString)), SLOT(init()));
    connect(d->serviceWatcher, SIGNAL(serviceUnregistered(QString)), SLOT(serviceUnregistered()));

    qDebug() << "OwncloudSettings module loaded.";
    connect(this, SIGNAL(foldersChanged()), SLOT(updateGlobalStatus()));
    init();
    //emit foldersChanged();
}

OwncloudSettings::~OwncloudSettings()
{
    //qDebug() << "owncloudsettings destroy";
    delete d;
}


void OwncloudSettings::init()
{
    if (!QDBusConnection::sessionBus().interface()->isServiceRegistered(QLatin1String("org.kde.owncloudsync"))) {
        setOwncloudStatus(OwncloudSettings::Error);
        setError(OwncloudSettings::NoDaemonError);
        d->globalStatus = OwncloudFolder::Error;
        QTimer::singleShot(2000, this, SIGNAL(globalStatusChanged()));
        delete d->client;
        d->client = 0; // invalidate client when dbus connection is lost
        return;
    } else if (d->client == 0) {
        qDebug() << "++ POC COnnecting up dbus";
        d->client = new OrgKdeOwncloudsyncInterface("org.kde.owncloudsync", "/", QDBusConnection::sessionBus(), this);
        QObject::connect(d->client, SIGNAL(statusChanged(int)), this, SLOT(setOwncloudStatus(int)));
        QObject::connect(d->client, SIGNAL(errorChanged(int)), this, SLOT(setError(int)));
        QObject::connect(d->client, SIGNAL(folderListChanged(const QVariantMap&)), this, SLOT(setFolderList(const QVariantMap&)));
        QObject::connect(d->client, SIGNAL(folderChanged(const QVariantMap&)), this, SLOT(setFolder(const QVariantMap&)));
        QObject::connect(d->client, SIGNAL(owncloudChanged(const QVariantMap&)), this, SLOT(setOwncloud(const QVariantMap&)));
        QObject::connect(d->client, SIGNAL(remoteFolderExists(const QString&, bool)), this, SIGNAL(remoteFolderExists(const QString&, bool)));
        //QObject::connect(d->client, SIGNAL(remoteFolderExists(const QString&, bool)), this, SLOT(slotRemoteFolderExists(const QString&, bool)));
        QObject::connect(d->client, SIGNAL(progressChanged(const QVariantMap&)), d->progress, SLOT(setProgress(const QVariantMap&)));
        QTimer::singleShot(2000, this, SLOT(refresh()));
    }
}

void OwncloudSettings::serviceUnregistered()
{
    // ditch caches
    foreach (OwncloudFolder *f, d->folders) {
        delete f;
    }
    d->folders.clear();
    delete d->client;
    d->client = 0;

    // reset internal caches and emit changed signals
    setOwncloud(QVariantMap());;
    setOwncloudStatus(OwncloudSettings::Error);
    setError(OwncloudSettings::NoDaemonError);
    emit foldersChanged();
}

// -- owncloud Info handling

void OwncloudSettings::setOwncloud(const QVariantMap& m)
{
    qDebug() << " vm " << m;
    d->owncloudInfo = m;
    emit editionChanged();
    emit versionChanged();
    emit urlChanged();
}

QString OwncloudSettings::edition() const
{
    return d->owncloudInfo["edition"].toString();
}

QString OwncloudSettings::version() const
{
    return d->owncloudInfo["version"].toString();
}

void OwncloudSettings::setUrl(const QString& u)
{
    Q_UNUSED(u);
    qWarning() << "setUrl is not supported yet.";
}

QString OwncloudSettings::url() const
{
    return d->owncloudInfo["url"].toString();

}

int OwncloudSettings::owncloudStatus() const
{
    return d->status;
}

void OwncloudSettings::setOwncloudStatus(int i)
{
    qDebug() << "POC blaat was / new" << statusString(d->status) << statusString(i) << i;
    //if (d->status != i) {
        d->status = i;
        qDebug() << " !!! setOwncloudStatus " << statusString(i);
        emit owncloudStatusChanged();
    //}
}

int OwncloudSettings::error() const
{
    return d->error;
}

void OwncloudSettings::setError(int i)
{
    if (d->error != i) {
        qDebug() << "Error is now " << errorString(i);
        d->error = i;
        emit errorChanged();
    }
}

void OwncloudSettings::setProgress(const QVariantMap& m)
{
    d->progress->setProgress(m);
    emit progressChanged();
}

SyncProgress* OwncloudSettings::progress() const
{
    return d->progress;
}


void OwncloudSettings::startDaemon()
{
    // start daemon
    qDebug() << "Start Daemon...";
    KProcess::startDetached("owncloudsyncd");
}

void OwncloudSettings::setupOwncloud(const QString& server, const QString& user, const QString& password)
{
    if (d->client) {
        qDebug() << "setting up owncloud: " << server << user << password;
        d->client->setupOwncloud(server, user, password);
    }
}


// -- Folder handling

void OwncloudSettings::setFolderList(const QVariantMap& m)
{
    //qDebug() << "Folder changed!" << m;
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
    //qDebug() << " Folder updated: " << m;
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
//         qDebug() << "OC Updating existing folder" << alias;
    } else {
        qDebug() << "OC New Folder" << alias;
        folder = new OwncloudFolder(this);
        d->folders << folder;
        connect(folder, SIGNAL(enableFolder(const QString&, bool)), this, SLOT(enableFolder(const QString&, bool)));
        connect(folder, SIGNAL(folderStatusChanged()), SLOT(updateGlobalStatus()));

        connect(folder, SIGNAL(syncFolder(const QString&)), d->client, SLOT(syncFolder(const QString&)));
        //connect(folder, SIGNAL(cancelFolder(const QString&)), d->client, SLOT(cancelSync(const QString&)));
    }
    folder->setDisplayName(alias);
    folder->setLocalPath(m["localPath"].toString());
    folder->setRemotePath(m["remotePath"].toString());
    folder->setFolderStatus(m["status"].toInt());
    folder->setErrorMessage(m["errorMessage"].toString());
    QDateTime dt;
    dt.setMSecsSinceEpoch(m["syncTime"].toULongLong());
    folder->setSyncTime(dt);
    //qDebug() << " === OC Updating" << alias << folder->folderStatus() << dt;
    if (!exists) {
        emit foldersChanged();
    }
}

void OwncloudSettings::enableFolder(const QString& name, bool enabled)
{
    if (d->client) {
        d->client->enableFolder(name, enabled);
    }
}

void OwncloudSettings::checkRemoteFolder(const QString &folder)
{
    if (d->client && !folder.isEmpty()) {
        d->client->checkRemoteFolder(folder);
    }
}

CreateFolderJob* OwncloudSettings::createRemoteFolder(const QString &folder)
{
    if (d->client && !folder.isEmpty()) {
        qDebug() << "creating remote folder" << folder;
        CreateFolderJob* j = new CreateFolderJob(folder, this);
        d->createFolderJobs[folder] = j;
        d->client->createRemoteFolder(folder);
        return j;
    }
    return 0;
}

bool OwncloudSettings::createLocalFolder(const QString& folder)
{
    QDir f = QDir();
    qDebug() << "mkdir : " << folder;
    return f.mkpath(folder);
}

void OwncloudSettings::enableAllFolders(bool enabled)
{
    foreach (OwncloudFolder* f, d->folders) {
        if (enabled) {
            f->enable();
        } else {
            f->disable();
        }
    }
}

void OwncloudSettings::addSyncFolder(const QString &localFolder, const QString &remoteFolder, const QString &aliasName)
{
    if (d->client) {
        qDebug() << " .. addSyncFolder: " << localFolder << remoteFolder << aliasName;
        d->client->addSyncFolder(localFolder, remoteFolder, aliasName);
    }
}

void OwncloudSettings::removeSyncFolder(const QString& alias)
{
    if (d->client) {
        qDebug() << "removeSyncFolder: " << alias;
        d->client->removeSyncFolder(alias);
        foreach (OwncloudFolder *f, d->folders) {

            if (f->displayName() == alias) {
                delete f;
                d->folders.removeAll(f);
                emit foldersChanged();
            }
        }
    }
}

QString OwncloudSettings::verifyFolder(const QString &localFolder, const QString &remoteFolder, const QString &alias)
{
    Q_UNUSED(remoteFolder);
    bool aError = false;
    bool lError = false;
    foreach (const OwncloudFolder *folder, d->folders) {
        if (folder->displayName() == alias) aError = true;
        if (folder->localPath() == localFolder) lError = true;
    }
    QString err;

    if (aError && lError) {
        err = i18n("The display name \"%1\" is already in use and the local folder \"%2\" already connected to ownCloud. ", alias, localFolder);
    }
    if (lError) {
        err = i18n("The local folder \"%1\" is already connected to ownCloud. ", localFolder);
    }
    if (aError) {
        err = i18n("The display name \"%1\" is already in use. ", alias);
    }
    //qDebug() << "verify: " << localFolder << remoteFolder << alias << err;
    return err;
}

bool OwncloudSettings::isConfigured(const QString &localFolder, const QString &remoteFolder, const QString &alias)
{

    foreach (const OwncloudFolder *folder, d->folders) {
//         qDebug() << " --- !!!!!!!";
//         qDebug() << "displayName" << alias << folder->displayName() << (folder->displayName() == alias);
//         qDebug() << "remotePath" << remoteFolder << folder->remotePath() << (folder->remotePath() == remoteFolder);
//         qDebug() << "displayName" << folder->localPath() << localFolder << (QDir(folder->localPath()) == QDir(localFolder));
        if ((folder->displayName() == alias) &&
            (QDir(folder->localPath()) == QDir(localFolder)) &&
            (folder->remotePath() == remoteFolder)) {
            qDebug() << "=========== Folder configured: " << folder->displayName();
            return true;
        }
    }
    return false;
}

void OwncloudSettings::updateGlobalStatus()
{
    int newState = OwncloudFolder::Idle;

    bool disabled = true;
    bool running = false;
    bool waiting = false;
    bool errorneus = false;

    foreach (OwncloudFolder *f, d->folders) {
        if (f->folderStatus() != OwncloudFolder::Disabled) {
            disabled = false;
        }
        if (f->folderStatus() == OwncloudFolder::Running) {
            running = true;
        }
        if (!running && f->folderStatus() == OwncloudFolder::Waiting) {
            waiting = true;
        }
        if (f->folderStatus() == OwncloudFolder::Error) {
            //errorneus = true; // FIXME: Makes UI too noisy, come up with some elegant way of displaying errors
        }
    }

    QString _d = "idle";
    if (disabled) {
        _d = "disabled";
        newState = OwncloudFolder::Disabled;
    }
    if (errorneus) {
        newState = OwncloudFolder::Error;
        _d = "error";
    }
    if (running) {
        newState = OwncloudFolder::Running;
        _d = "running";
    }
    if (waiting) {
        newState = OwncloudFolder::Waiting;
        _d = "waiting";
    }
    if (d->error == OwncloudSettings::NoDaemonError) {
        newState = OwncloudFolder::Error;
    }

    if (d->globalStatus != newState) {
        d->globalStatus = newState;
        //qDebug() << "globalStatusChanged: " << _d;
        emit globalStatusChanged();
    }
}

int OwncloudSettings::globalStatus() const
{
    return  d->globalStatus;
}

QDeclarativeListProperty<OwncloudFolder> OwncloudSettings::folders()
{
    return QDeclarativeListProperty<OwncloudFolder>(this, d->folders);
}

void OwncloudSettings::refresh()
{
    if (d->client) {
        qDebug() << "POC refresh()";
        d->client->refresh();
    }
}

MiniJob* OwncloudSettings::createMiniJob()
{
    MiniJob* mj = new MiniJob(this);
    return mj;
}

void OwncloudSettings::openConfig()
{
    if (KStandardDirs::findExe("active-settings") != QString()) {
        qDebug() << "Found active-settings, using that as shell: active-settings org.kde.active.settings.owncloud";
        KRun::runCommand("active-settings org.kde.active.settings.owncloud", 0);
    } else {
        qDebug() << "Opening kcmshell4 owncloudconfig";
        KRun::runCommand("kcmshell4 owncloudconfig", 0);
    }
}

void OwncloudSettings::slotRemoteFolderExists(const QString &folder, bool exists)
{
    if (d->createFolderJobs.keys().contains(folder)) {
        qDebug() << " job and folder exists" << exists;
        d->createFolderJobs[folder]->setResult(exists);
    } else {
        qDebug() << "!createjob not found for folder : " << folder;
    }
    emit remoteFolderExists(folder, exists);
}

QString OwncloudSettings::errorString(int e)
{
    /*
        enum Error {
            NoError, // We're fine
            AuthenticationError, // owncloud server doesn't accept credentials
            NetworkError, // server is unreachable
            NoConfigurationError, // no configuration found
            NoDaemonError, // owncloudsyncd is not running
            CustomError // Anything else
        };
    */
    QString o;
    if (e == NoError) {
        o = "NoError";
    } else if (e == AuthenticationError) {
        o = "AuthenticationError";
    } else if (e == NetworkError) {
        o = "NetworkError";
    } else if (e == NoConfigurationError) {
        o = "NoConfigurationError";
    } else if (e == NoDaemonError) {
        o = "NoDaemonError";
    } else if (e == CustomError) {
        o = "CustomError";
    }

    return o;
}

QString OwncloudSettings::statusString(int s)
{
    /*
        enum Status {
            Disconnected,
            Connected,
            Error
        };
    */
    QString o;
    if (s == Disconnected) {
        o = "Disconnected";
    } else if (s == Connected) {
        o = "Connected";
    } else if (s == Error) {
        o = "Error";
    }

    return o;
}


#include "owncloudsettings.moc"
