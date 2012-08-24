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
#include "owncloudsettings.h"

#include <qdebug.h>
//#include <QMessageBox>
#include <QVariant>
#include <QTimer>

#include "mirall/syncresult.h"
#include "mirall/folder.h"
#include "mirall/folderman.h"
#include "mirall/logbrowser.h"
#include "mirall/mirallconfigfile.h"
#include "mirall/owncloudinfo.h"

class OwncloudSyncDaemonPrivate {
public:
    OwncloudSyncDaemon *q;
    QString name;
    QString localPath;
    QString remotePath;
    QString display;

    QVariantMap folderList;
    QVariantMap owncloudInfo;
    QHash<QString, QVariantMap> folders;

    Mirall::FolderMan* folderMan;
    Mirall::ownCloudInfo* ocInfo;
    QString configHandle;

    int ocStatus;
    int ocError;

    void loadFolders();

    int c;
};

OwncloudSyncDaemon::OwncloudSyncDaemon(QObject *parent)
    : QObject(parent)
{
    d = new OwncloudSyncDaemonPrivate;
    d->q = this;
    d->display = "";

    d->ocStatus = OwncloudSettings::Disconnected;
    d->ocError = OwncloudSettings::NoError;

    d->folderMan = new Mirall::FolderMan();
    connect( d->folderMan, SIGNAL(folderSyncStateChange(QString)),
             this,SLOT(slotSyncStateChange(QString)));
    connect( d->folderMan, SIGNAL(folderSyncStateChange(QNetworkReply*)),
             this,SLOT(slotSyncStateChange(QNetworkReply)));

    d->ocInfo = Mirall::ownCloudInfo::instance();
    //d->ocInfo->setCustomConfigHandle("mirall");
    connect(d->ocInfo,SIGNAL(ownCloudInfoFound(QString,QString,QString,QString)),
             SLOT(slotOwnCloudFound(QString,QString,QString,QString)));

    connect(d->ocInfo,SIGNAL(noOwncloudFound(QNetworkReply*)),
             SLOT(slotNoOwnCloudFound(QNetworkReply*)));

    connect(d->ocInfo,SIGNAL(ownCloudDirExists(QString,QNetworkReply*)),
             this,SLOT(slotAuthCheck(QString,QNetworkReply*)));

    connect(d->ocInfo, SIGNAL(ownCloudDirExists(QString,QNetworkReply*)),
             SLOT(slotDirCheckReply(QString,QNetworkReply*)));

    connect(d->ocInfo, SIGNAL(webdavColCreated(QNetworkReply::NetworkError)),
             SLOT(slotCreateRemoteFolderFinished(QNetworkReply::NetworkError)));

    //refresh();

    if(d->ocInfo->isConfigured() ) {
        d->ocInfo->checkInstallation();
        d->loadFolders();
    }
    //qDebug() << "OwncloudSyncDaemon loaded.";
}

OwncloudSyncDaemon::~OwncloudSyncDaemon()
{
    delete d;
}

void OwncloudSyncDaemon::slotSyncStateChange(const QString &s )
{
//     qDebug() << "OC slotSyncStateChange : " << s;
    updateFolder(d->folderMan->folder(s));
}

QString OwncloudSyncDaemon::display()
{
    qDebug() << "OC display() " << d->display;
    return d->display;
}

QVariantMap OwncloudSyncDaemon::folder(QString name)
{
    qDebug() << "OC folder()" << name;
    return d->folders[name];
}

void OwncloudSyncDaemon::enableFolder(const QString &name, bool enabled)
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

QVariantMap OwncloudSyncDaemon::folderList()
{
    return d->folderList;
}

void OwncloudSyncDaemon::refresh()
{
    qDebug() << "OC Syncdaemon refresh()";
    if (!d->ocInfo->isConfigured()) {
        d->ocStatus = OwncloudSettings::Error;
        d->ocError = OwncloudSettings::NoConfigurationError;
    }
    d->loadFolders();
    emit statusChanged(d->ocStatus);
    emit errorChanged(d->ocError);
    emit owncloudChanged(d->owncloudInfo);
}

void OwncloudSyncDaemonPrivate::loadFolders()
{
    //kDebug() << "Loaded folders : " << folders.count();
    QStringList fs;
    qDebug() << "Loading folders";
    foreach (Mirall::Folder* f, folderMan->map()) {
        qDebug() << "New folder: " << f->alias() << f->path() << f->secondPath();
        //fs << f->alias();
        q->updateFolder(f);
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

void OwncloudSyncDaemon::updateFolder(const Mirall::Folder* folder)
{
    QVariantMap m;
    m["name"] = folder->alias();
    m["localPath"] = folder->path();

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
        //qDebug() << "OC sync enabled? " << folder->alias() << folder->syncEnabled();
    }
//     qDebug() << "OC sync enabled? " << folder->alias() << folder->syncEnabled();
    //qDebug() << "OC updateFolder: " << errorMsg(r);
    if (r == 999) {
        //qDebug() << "OC dunno what to do with " << r;
        s = OwncloudFolder::Error;
    }
    m["status"] = s;

    if (s == OwncloudFolder::Error) {
        m["errorMessage"] = folder->syncResult().errorString();
    } else {
        m["errorMessage"] = QString();
    }

    //d->folderList[folder->alias()] = s;

    d->folders[folder->alias()] = m;
    emit folderChanged(m);
}

void OwncloudSyncDaemon::addSyncFolder(const QString& localFolder, const QString& remoteFolder, const QString& alias)
{
    d->folderMan->addFolderDefinition(QString("owncloud"), alias, localFolder, remoteFolder, false);

    qDebug() << "OCD OwncloudSyncDaemon::addSyncFolder: " << localFolder << remoteFolder << alias;

    d->folderMan->setupFolders();
    refresh();
}

void OwncloudSyncDaemon::checkRemoteFolder(const QString& f)
{
    qDebug() << "OC Querying folder " << f;
    d->ocInfo->getWebDAVPath(f);

}

void OwncloudSyncDaemon::slotDirCheckReply(const QString &url, QNetworkReply *reply)
{
    qDebug() << "OC Got reply from owncloud dir check: " << url << " :" << reply->error();
    //_dirChecked = (reply->error() == QNetworkReply::NoError);
    if(reply->error() == QNetworkReply::NoError) {
        qDebug() << "OC " << "The folder is exists.";
    } else {
        qDebug() << "OC " << "The folder is not available on your ownCloud.";
    }
    emit remoteFolderExists(url, reply->error() == QNetworkReply::NoError);

    //emit completeChanged();
}

void OwncloudSyncDaemon::createRemoteFolder(const QString &f)
{
    if(f.isEmpty()) return;

    qDebug() << "OC creating folder on ownCloud: " << f;
    d->ocInfo->mkdirRequest(f);
}

void OwncloudSyncDaemon::slotCreateRemoteFolderFinished(QNetworkReply::NetworkError error)
{
  qDebug() << "OC ** webdav mkdir request finished " << error;

  //_ui.OCFolderLineEdit->setEnabled( true );
  // the webDAV server seems to return a 202 even if mkdir was successful.
  if (error == QNetworkReply::NoError || error == QNetworkReply::ContentOperationNotPermittedError) {
    qDebug() << "OC " << "Folder on ownCloud was successfully created.";
    //slotTimerFires();
  } else {
    //showWarn( tr("Failed to create the folder on ownCloud.<br/>Please check manually."), false );
  }
}


void OwncloudSyncDaemon::slotOwnCloudFound( const QString& url, const QString& versionStr, const QString& version, const QString& edition)
{
    qDebug() << "OCD ** Application: ownCloud found: " << url << " with version " << versionStr << "(" << version << ")";
    // now check the authentication
    Mirall::MirallConfigFile cfgFile;
    cfgFile.setOwnCloudVersion( version );

    d->owncloudInfo["url"] = url;
    d->owncloudInfo["version"] = version;
    d->owncloudInfo["versionString"] = versionStr;
    d->owncloudInfo["edition"] = edition;

    d->ocStatus = OwncloudSettings::Disconnected;
    d->ocError = OwncloudSettings::NoError;

    emit owncloudChanged(d->owncloudInfo);
    emit statusChanged(d->ocStatus);
    emit errorChanged(d->ocError);

    QTimer::singleShot( 0, this, SLOT( slotCheckAuthentication() ));
}

void OwncloudSyncDaemon::slotNoOwnCloudFound( QNetworkReply* reply )
{
    qDebug() << "OCD ** Application: NO ownCloud found!";
    QString msg;
    if( reply ) {
        QString url( reply->url().toString() );
        url.remove( "/status.php" );
        msg = tr("<p>The ownCloud at %1 could not be reached.</p>").arg( url );
        msg += tr("<p>The detailed error message is<br/><tt>%1</tt></p>").arg( reply->errorString() );
    }
    msg += tr("<p>Please check your configuration by clicking on the tray icon.</p>");

    QString er = tr("ownCloud Connection Failed");

    d->ocStatus = OwncloudSettings::Error;
    d->ocError = OwncloudSettings::NoConfigurationError;
    emit statusChanged(d->ocStatus);
    emit errorChanged(d->ocError);
}

void OwncloudSyncDaemon::slotCheckAuthentication()
{
    qDebug() << "OC slotCheckAuthentication";
    qDebug() << "# checking for authentication settings.";
    d->ocInfo->getRequest("/", true ); // this call needs to be authenticated.
}

void OwncloudSyncDaemon::slotAuthCheck( const QString& ,QNetworkReply *reply )
{
    qDebug() << "OC slotAuthCheck";
    if( reply->error() == QNetworkReply::AuthenticationRequiredError ) { // returned if the user is wrong.
        qDebug() << "OC ******** Password is wrong!";
        d->ocStatus = OwncloudSettings::Error;
        d->ocError = OwncloudSettings::AuthenticationError;
        emit statusChanged(d->ocStatus);
        emit errorChanged(d->ocError);
    } else if( reply->error() == QNetworkReply::OperationCanceledError ) {
        // the username was wrong and ownCloudInfo was closing the request after a couple of auth tries.
        qDebug() << "OC ******** Username or password is wrong!";
        //emit statusMessageChanged(er);
        d->ocStatus = OwncloudSettings::Error;
        d->ocError = OwncloudSettings::AuthenticationError;
        emit statusChanged(d->ocStatus);
        emit errorChanged(d->ocError);
    } else {
        qDebug() << "OC ######## Credentials are ok!";
        d->ocStatus = OwncloudSettings::Connected;
        d->ocError = OwncloudSettings::NoError;
        emit statusChanged(d->ocStatus);
        emit errorChanged(d->ocError);
        d->folderMan->setupFolders();
        d->loadFolders();
    }
}

void OwncloudSyncDaemon::setupOwncloud(const QString &server, const QString &user, const QString &password)
{
    Mirall::MirallConfigFile cfgFile(d->configHandle);
    cfgFile.setRemotePollIntval(60000);

    cfgFile.writeOwncloudConfig(QLatin1String("ownCloud"), server, user, password, false);
    qDebug() << "OC Setting up: " << server << user << password;
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
    } else {
        qDebug() << " OC  ownCloud seems not configured.";
    }
}

#include "owncloudsyncdaemon.moc"
