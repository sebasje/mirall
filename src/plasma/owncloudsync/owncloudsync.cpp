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
#include "owncloudfolder.h"
#include "owncloudsettings.h"

#include <KIO/AccessManager>
#include <kdebug.h>
#include <QVariant>
#include <QTimer>

#include "mirall/credentialstore.h"
#include "mirall/syncresult.h"
#include "mirall/folder.h"
#include "mirall/mirallconfigfile.h"

class OwncloudSyncPrivate {
public:
    OwncloudSync *q;
    QString name;
    QString localPath;
    QString remotePath;
    QString display;

    QVariantMap folderList;
    QVariantMap owncloudInfo;
    QHash<QString, QVariantMap> folders;
    QHash<QString, QDateTime> syncTime;

    Mirall::FolderMan* folderMan;
    Mirall::ownCloudInfo* ocInfo;
    QString configHandle;
    QTimer *delay;
    int ocStatus;
    int ocError;

    int c;
};

OwncloudSync::OwncloudSync(QObject *parent)
    : QObject(parent)
{
    d = new OwncloudSyncPrivate;
    d->q = this;
    d->display = "";
    d->delay = 0;
    d->ocStatus = OwncloudSettings::Disconnected;
    d->ocError = OwncloudSettings::NoError;
    d->ocInfo = Mirall::ownCloudInfo::instance();
// //     KIO::AccessManager *nam = new KIO::AccessManager(this);
// //     kDebug() << "OC Seetting KIO::NAM";
// //     d->ocInfo->setNetworkAccessManager(nam);
    init();

    slotFetchCredentials();
}

void OwncloudSync::init()
{
    d->folderMan = new Mirall::FolderMan();
    connect( d->folderMan, SIGNAL(folderSyncStateChange(const QString&)),
             this,SLOT(slotSyncStateChange(const QString&)));

    //d->ocInfo->setCustomConfigHandle("mirall");
    connect(d->ocInfo,SIGNAL(ownCloudInfoFound(QString,QString,QString,QString)),
             SLOT(slotOwnCloudFound(QString,QString,QString,QString)));

    connect(d->ocInfo,SIGNAL(noOwncloudFound(QNetworkReply*)),
             SLOT(slotNoOwnCloudFound(QNetworkReply*)));

    kDebug() << "OC connecting to slotAuthCheck";
    connect(d->ocInfo,SIGNAL(ownCloudDirExists(QString,QNetworkReply*)),
             this,SLOT(slotAuthCheck(QString,QNetworkReply*)));

    connect(d->ocInfo, SIGNAL(ownCloudDirExists(QString,QNetworkReply*)),
             SLOT(slotDirCheckReply(QString,QNetworkReply*)));

}

OwncloudSync::~OwncloudSync()
{
    delete d;
}

Mirall::FolderMan* OwncloudSync::folderMan()
{
    return d->folderMan;
}

Mirall::ownCloudInfo* OwncloudSync::ocInfo()
{
    return d->ocInfo;
}

void OwncloudSync::slotSyncStateChange(const QString &s)
{
    Mirall::Folder *f = d->folderMan->folder(s);
    if (f && (f->syncResult().status() == Mirall::SyncResult::Success)) {
        d->syncTime[s] = QDateTime::currentDateTime();
//         qDebug() << "OC updated syncTime for " << s << d->syncTime[s];
    }
    updateFolder(d->folderMan->folder(s));
}

QString OwncloudSync::display()
{
    //qDebug() << "OC display() " << d->display;
    return d->display;
}

QVariantMap OwncloudSync::folder(QString name)
{
    //qDebug() << "OC folder()" << name;
    return d->folders[name];
}

void OwncloudSync::enableFolder(const QString &name, bool enabled)
{
    qDebug() << " OC enableFolder: " << name << enabled;
    Mirall::Folder *f = d->folderMan->folder(name);
    if (f) {
        f->setSyncEnabled(enabled);
        updateFolder(f);
    } else {
        qWarning() << "Folder \"" << name << "\" does not exist.";
    }
}

void OwncloudSync::syncFolder(const QString& name)
{
    qDebug() << " OC syncFolder: " << name;
    Mirall::Folder *f = d->folderMan->folder(name);
    if (f) {
        f->startSync(QStringList());

    } else {
        qWarning() << "Folder \"" << name << "\" does not exist.";
    }
}

void OwncloudSync::cancelSync(const QString& name)
{
    qWarning() << "OC Terminating a sync operation is not implemented yet.";
    //qDebug() << " OC syncFolder: " << name;
    Mirall::Folder *f = d->folderMan->folder(name);
    if (f) {
        qWarning() << "Terminating a sync operation is not implemented yet.";
    } else {
        qWarning() << "Folder \"" << name << "\" does not exist.";
    }
}

QVariantMap OwncloudSync::folderList()
{
    return d->folderList;
}

void OwncloudSync::refresh()
{
    qDebug() << "OC Syncdaemon refresh()";
    if (!d->ocInfo->isConfigured()) {
        d->ocStatus = OwncloudSettings::Error;
        d->ocError = OwncloudSettings::NoConfigurationError;
    } else {
        loadFolders();
    }
    emit statusChanged(d->ocStatus);
    emit errorChanged(d->ocError);
    emit owncloudChanged(d->owncloudInfo);
}

void OwncloudSync::loadFolders()
{
    if (d->ocStatus == OwncloudSettings::Connected) {
        QStringList fs;
        foreach (Mirall::Folder* f, d->folderMan->map()) {
            //qDebug() << "OC New folder: " << f->alias() << f->path() << f->secondPath();
            //fs << f->alias();
            connect(f, SIGNAL(syncFinished(Mirall::SyncResult)), SLOT(folderSyncFinished(Mirall::SyncResult)));
            updateFolder(f);
        }
    } else {
        d->folderList.clear();
        d->folders.clear();
        emit folderListChanged(d->folderList);
    }
}

void OwncloudSync::folderSyncFinished(Mirall::SyncResult r)
{
    if (r.status() == Mirall::SyncResult::Success) {
        Mirall::Folder *f = static_cast<Mirall::Folder*>(sender());
        if (f) {
            d->syncTime[f->alias()] = QDateTime::currentDateTime();
        } else {
            //qDebug() << " OC no folder found";
        }
    }
}

QString errorMsg(int r) {
    QString s;
    if (r == Mirall::SyncResult::Success) s = "Mirall::SyncResult::Success -> OwncloudFolder::Idle";
    if (r == Mirall::SyncResult::Error) s = "Mirall::SyncResult::Error -> OwncloudFolder::Error";
    if (r == Mirall::SyncResult::NotYetStarted) s = "Mirall::SyncResult::NotYetStarted -> OwncloudFolder::Waiting";
    if (r == Mirall::SyncResult::SyncRunning) s = "Mirall::SyncResult::SyncRunning -> OwncloudFolder::Running";
    if (r == Mirall::SyncResult::SetupError) s = "Mirall::SyncResult::SetupError -> OwncloudFolder::Error";
    if (r == Mirall::SyncResult::Undefined) s = "Mirall::SyncResult::Undefined -> OwncloudFolder::Error";
    return s;
}

void OwncloudSync::updateFolder(const Mirall::Folder* folder)
{
    QVariantMap m;
    m["name"] = folder->alias();
    m["localPath"] = folder->path();
    m["remotePath"] = folder->secondPath();
    m["syncTime"] = d->syncTime[folder->alias()].toMSecsSinceEpoch();
//     qDebug() << "OC updateFolder:: path, secondPath: " << folder->path() << ", " << d->syncTime[folder->alias()];

    int s = 999;
    int r = folder->syncResult().status();
    //qDebug() << " c" << c << " r" << r;
    if (folder->syncEnabled()) {
        if (r == Mirall::SyncResult::Success) s = OwncloudFolder::Idle;
        if (r == Mirall::SyncResult::Error) s = OwncloudFolder::Error;
        if (r == Mirall::SyncResult::NotYetStarted) s = OwncloudFolder::Waiting;
        if (r == Mirall::SyncResult::SyncRunning) s = OwncloudFolder::Running;
        if (r == Mirall::SyncResult::SetupError) s = OwncloudFolder::Error;
        if (r == Mirall::SyncResult::Undefined) s = OwncloudFolder::Error;
    } else {
        s = OwncloudFolder::Disabled;
    }
    if (r == 999) {
        s = OwncloudFolder::Error;
    }
    m["status"] = s;

    if (s == OwncloudFolder::Error) {
        m["errorMessage"] = folder->syncResult().errorString();
    } else {
        m["errorMessage"] = QString();
    }

    d->folders[folder->alias()] = m;
    //qDebug() << " OC FOLdERs: " << folder->alias() << folder->syncResult().statusString();
    emit folderChanged(m);
}

void OwncloudSync::addSyncFolder(const QString& localFolder, const QString& remoteFolder, const QString& alias)
{
    d->folderMan->addFolderDefinition(QString("owncloud"), alias, localFolder, remoteFolder, false);

    qDebug() << "OCD OwncloudSyncDaemon::addSyncFolder: " << localFolder << remoteFolder << alias;

    d->folderMan->setupFolders();
    refresh();
}

void OwncloudSync::removeSyncFolder(const QString& alias)
{
    d->folderMan->slotRemoveFolder(alias);

    qDebug() << "OCD OwncloudSyncDaemon::removeSyncFolder: " << alias;

    if (!d->delay) {
        d->delay = new QTimer(this);
        d->delay->setSingleShot(true);
        d->delay->setInterval(2000);
        connect(d->delay, SIGNAL(timeout()), SLOT(delayedReadConfig()));
    }
    d->delay->start();
    d->folders.remove(alias);
    d->folderMan->setupFolders();
    refresh();
}

void OwncloudSync::delayedReadConfig()
{
    d->folderMan->setupFolders();
    refresh();
}

void OwncloudSync::checkRemoteFolder(const QString& f)
{
    if (d->ocStatus == OwncloudSettings::Connected) {
        QNetworkReply* reply = d->ocInfo->getWebDAVPath(f);
        connect(reply, SIGNAL(finished()), SLOT(slotCheckRemoteFolderFinished()));
        //connect(reply, SIGNAL(finished()), SLOT(slotCheckRemoteFolderFinished()));
    }
}

void OwncloudSync::slotCheckRemoteFolderFinished()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    bool exists = reply->error() == QNetworkReply::NoError;
    QString p = reply->url().toString().split("remote.php/webdav/")[1];
    emit remoteFolderExists(p, exists);
}

void OwncloudSync::slotDirCheckReply(const QString &url, QNetworkReply *reply)
{
    //qDebug() << " OC slotDirCheckReply" << url <<  (reply->error() == QNetworkReply::NoError) << (int)(reply->error());
    emit remoteFolderExists(url, reply->error() == QNetworkReply::NoError);
}

void OwncloudSync::createRemoteFolder(const QString &f)
{
    if(f.isEmpty()) return;

    qDebug() << "OC creating folder on ownCloud: " << f;
    QNetworkReply* reply = d->ocInfo->mkdirRequest(f);
    connect(reply, SIGNAL(finished()), SLOT(slotCreateRemoteFolderFinished()));
    //connect(reply, SIGNAL(error()), SLOT(slotCreateRemoteFolderFinished()));

}

void OwncloudSync::slotCreateRemoteFolderFinished()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    bool exists = reply->error() == QNetworkReply::NoError;
    QString p = reply->url().toString().split("remote.php/webdav/")[1];
    qDebug() << " === OC slot -- CREATE -- RemoteFolderFinished() : " << reply->url() << reply->error() << p << exists;
    emit remoteFolderExists(p, exists);
}

void OwncloudSync::slotOwnCloudFound( const QString& url, const QString& versionStr, const QString& version, const QString& edition)
{
    qDebug() << "OCD : ownCloud found: " << url << " with version " << versionStr << "(" << version << ")";
    // now check the authentication

    Mirall::MirallConfigFile cfgFile(d->configHandle);
    cfgFile.setOwnCloudVersion( version );
    qDebug() << " OC polling interval: " << cfgFile.remotePollInterval();

    d->owncloudInfo["url"] = url;
    d->owncloudInfo["version"] = version;
    d->owncloudInfo["versionString"] = versionStr;
    d->owncloudInfo["edition"] = edition;

    d->ocStatus = OwncloudSettings::Disconnected;
    d->ocError = OwncloudSettings::NoError;

//     emit owncloudChanged(d->owncloudInfo);
//     emit statusChanged(d->ocStatus);
//     emit errorChanged(d->ocError);

}

void OwncloudSync::slotFetchCredentials()
{
    QString trayMessage;

    if( Mirall::CredentialStore::instance()->canTryAgain() ) {
        connect( Mirall::CredentialStore::instance(), SIGNAL(fetchCredentialsFinished(bool)),
                 this, SLOT(slotCredentialsFetched(bool)) );
        Mirall::CredentialStore::instance()->fetchCredentials();
//         if( Mirall::CredentialStore::instance()->state() == Mirall::CredentialStore::TooManyAttempts ) {
//             trayMessage = tr("Too many attempts to get a valid password.");
//         }
    } else {
        qDebug() << "OC Can not try again to fetch Credentials.";
//          trayMessage = tr("ownCloud user credentials are wrong. Please check configuration.");
    }
    // FIXME: do anything more?
}


void OwncloudSync::slotCredentialsFetched(bool ok)
{
    qDebug() << "OC Credentials successfully fetched: " << ok;
    QString trayMessage;
    if( ! ok ) {
        trayMessage = tr("Error: Could not retrieve the password!");
        if( Mirall::CredentialStore::instance()->state() == Mirall::CredentialStore::UserCanceled ) {
            trayMessage = tr("Password dialog was canceled!");
        } else {
            trayMessage = Mirall::CredentialStore::instance()->errorMessage();
        }

        if( !trayMessage.isEmpty() ) {
            //_tray->showMessage(tr("Credentials"), trayMessage);
        }

        qDebug() << "OC Could not fetch credentials" << trayMessage;
    } else {
        // Credential fetched ok.
        const QString _url = d->owncloudInfo["url"].toString();
        const QString _u = Mirall::CredentialStore::instance()->user();
        const QString _p = Mirall::CredentialStore::instance()->password();
//         qDebug() << "OC User / password === " << _u << _p << _url;
//         qDebug() << "OC slotCredentialsFetched" << trayMessage;
        if(d->ocInfo->isConfigured() ) {
            d->ocInfo->checkInstallation();
            loadFolders();
        }
        QTimer::singleShot(0, this, SLOT(slotCheckAuthentication()));
    }
    disconnect( Mirall::CredentialStore::instance(), SIGNAL(fetchCredentialsFinished(bool)) );
}



void OwncloudSync::slotNoOwnCloudFound(QNetworkReply* reply)
{
    d->ocStatus = OwncloudSettings::Error;
    d->ocError = OwncloudSettings::NoConfigurationError;
    emit statusChanged(d->ocStatus);
    emit errorChanged(d->ocError);
}

void OwncloudSync::slotCheckAuthentication()
{
    kDebug() << "OwncloudSync::slotCheckAuthentication()";
    d->ocInfo->getRequest("/", true ); // this call needs to be authenticated.
}

void OwncloudSync::slotAuthCheck( const QString& ,QNetworkReply *reply )
{
    if( reply->error() == QNetworkReply::AuthenticationRequiredError ) { // returned if the user is wrong.
        if (d->ocStatus != OwncloudSettings::Error ||
                            d->ocError != OwncloudSettings::AuthenticationError) {
            qDebug() << "OC ******** Password is wrong!";
            d->ocStatus = OwncloudSettings::Error;
            d->ocError = OwncloudSettings::AuthenticationError;
            emit statusChanged(d->ocStatus);
            emit errorChanged(d->ocError);
        }
    } else if( reply->error() == QNetworkReply::OperationCanceledError ) {
        // the username was wrong and ownCloudInfo was closing the request after a couple of auth tries.
        qDebug() << "OC ******** Username or password is wrong!";
        if (d->ocStatus != OwncloudSettings::Error ||
                            d->ocError != OwncloudSettings::AuthenticationError) {
            d->ocStatus = OwncloudSettings::Error;
            d->ocError = OwncloudSettings::AuthenticationError;
            emit statusChanged(d->ocStatus);
            emit errorChanged(d->ocError);
        }
    } else {
//         qDebug() << "OC ######## Credentials are ok!";
        if (d->ocStatus != OwncloudSettings::Connected) {
            qDebug() << "OC ****** changing to Connected/NoError! :-)";
            d->ocStatus = OwncloudSettings::Connected;
            d->ocError = OwncloudSettings::NoError;
            emit statusChanged(d->ocStatus);
            emit errorChanged(d->ocError);
            d->folderMan->setupFolders();
            loadFolders();
        }
    }
}

void OwncloudSync::setupOwncloud(const QString &server, const QString &user, const QString &password)
{
    Mirall::MirallConfigFile cfgFile(d->configHandle);
    cfgFile.setRemotePollIntval(600000); // ten minutes for now

    bool https = server.startsWith("https");

    QString _srv = server;
    if (!server.endsWith('/')) {
        _srv.append('/');
    }

    cfgFile.writeOwncloudConfig(QLatin1String("ownCloud"), _srv, user, password, https, false);
    Mirall::CredentialStore::instance()->saveCredentials();
    kDebug() << "OC - - - - -  Setting up OwnCloud: " << _srv << user << password << https;
    cfgFile.acceptCustomConfig();

    if( d->folderMan ) {
        d->folderMan->removeAllFolderDefinitions();
    }

    d->configHandle.clear();
    d->ocInfo->setCustomConfigHandle(QString());

    d->ocInfo->setCustomConfigHandle(d->configHandle);
    if (d->ocInfo->isConfigured()) {
        // reset the SSL Untrust flag to let the SSL dialog appear again.
        d->ocInfo->resetSSLUntrust();
        d->ocInfo->checkInstallation();
        loadFolders();
    } else {
        kDebug() << " OC  ownCloud seems not configured.";
    }
    QTimer::singleShot(0, this, SLOT(slotCheckAuthentication()));
}


#include "owncloudsync.moc"
