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
    QHash<QString, QVariantMap> folders;
    QTimer *timer;

    Mirall::FolderMan* folderMan;
    Mirall::ownCloudInfo* ocInfo;

    void loadFolders();

    int c;
};

OwncloudSyncDaemon::OwncloudSyncDaemon(QObject *parent)
{
    d = new OwncloudSyncDaemonPrivate;
    d->q = this;
    d->display = "ownCloud Sync Daemon";

    d->folderMan = new Mirall::FolderMan();
    connect( d->folderMan, SIGNAL(folderSyncStateChange(QString)),
             this,SLOT(slotSyncStateChange(QString)));

    d->ocInfo = new Mirall::ownCloudInfo( QString(), this );
    //d->ocInfo->setCustomConfigHandle("mirall");
    connect(d->ocInfo,SIGNAL(ownCloudInfoFound(QString,QString,QString,QString)),
             SLOT(slotOwnCloudFound(QString,QString,QString,QString)));

    connect(d->ocInfo,SIGNAL(noOwncloudFound(QNetworkReply*)),
             SLOT(slotNoOwnCloudFound(QNetworkReply*)));

    connect(d->ocInfo,SIGNAL(ownCloudDirExists(QString,QNetworkReply*)),
             this,SLOT(slotAuthCheck(QString,QNetworkReply*)));


    if(d->ocInfo->isConfigured() ) {
        d->ocInfo->checkInstallation();
    } else {
        QString er = tr("No ownCloud Configuration"),
                                tr("<p>No server connection has been configured for this ownCloud client.</p>"
                                "<p>Please right click on the ownCloud system tray icon and select <i>Configure</i> "
                                "to connect this client to an ownCloud server.</p>");
        // It was evaluated to open the config dialog from here directly but decided
        // against because the user does not know why. The popup gives a better user
        // guidance, even if its a click more.
        qDebug() << " Error: " << er;
        d->ocInfo->checkInstallation();
    }

    //int cnt = d->folderMan->setupFolders();
//     int cnt;
//     qDebug() << " no: " << cnt;

    //d->loadFolders();

    d->timer = new QTimer(this);
    d->c = 0;
    d->timer->setInterval(500);
    d->timer->setSingleShot(true);
    d->timer->start();
    connect(d->timer, SIGNAL(timeout()), SLOT(timeout()));
    //emit folderListChanged(d->folderList);
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
    emit statusMessageChanged(QString("Timeout has fired " + QString::number(d->c/5) + " times."));
    d->c++;

    d->loadFolders();
    emit folderListChanged(d->folderList);
    //qDebug() << d->folderList;
}

void OwncloudSyncDaemon::slotSyncStateChange(const QString &s )
{
//     qDebug() << "OC slotSyncStateChange : " << s;
    //d->loadFolders();
    updateFolder(d->folderMan->folder(s));
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
    qDebug() << "Loading folders";
    foreach (Mirall::Folder* f, folderMan->map()) {
        qDebug() << "New folder: " << f->alias() << f->path() << f->secondPath();
        //fs << f->alias();
        q->updateFolder(f);
    }
//     //fs << "Nirvana" << "Pearl Jam" << "Lana del Rey" << "Placebo" << "Cash";
//     foreach (const QString &_f, fs) {
//         int s = 0;
//         int r = c % 4;
//         //qDebug() << " c" << c << " r" << r;
//         if (r == 0) s = OwncloudFolder::Idle;
//         if (r == 1) s = OwncloudFolder::Disabled;
//         if (r == 2) s = OwncloudFolder::Waiting;
//         if (r == 3) s = OwncloudFolder::Error;
// 
//         folderList[_f] = s;
// 
//         QVariantMap m;
//         m["name"] = _f;
//         m["localPath"] = "/home/sebas/" + _f;
//         folders[name] = m;
//         c++;
//     }
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
    }
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

void OwncloudSyncDaemon::slotOwnCloudFound( const QString& url, const QString& versionStr, const QString& version, const QString& edition)
{
    qDebug() << "OC ** Application: ownCloud found: " << url << " with version " << versionStr << "(" << version << ")";
    // now check the authentication
    Mirall::MirallConfigFile cfgFile;
    cfgFile.setOwnCloudVersion( version );

    QTimer::singleShot( 0, this, SLOT( slotCheckAuthentication() ));
}

void OwncloudSyncDaemon::slotNoOwnCloudFound( QNetworkReply* reply )
{
    qDebug() << "OC ** Application: NO ownCloud found!";
    QString msg;
    if( reply ) {
        QString url( reply->url().toString() );
        url.remove( "/status.php" );
        msg = tr("<p>The ownCloud at %1 could not be reached.</p>").arg( url );
        msg += tr("<p>The detailed error message is<br/><tt>%1</tt></p>").arg( reply->errorString() );
    }
    msg += tr("<p>Please check your configuration by clicking on the tray icon.</p>");

    QString er = tr("ownCloud Connection Failed");
    //_actionAddFolder->setEnabled( false );
    //setupContextMenu();
}

void OwncloudSyncDaemon::slotCheckAuthentication()
{
    qDebug() << "OC slotCheckAuthentication";
    qDebug() << "# checking for authentication settings.";
    d->ocInfo->getRequest("/", true ); // this call needs to be authenticated.
    // simply GET the webdav root, will fail if credentials are wrong.
    // continue in slotAuthCheck here :-)
}

void OwncloudSyncDaemon::slotAuthCheck( const QString& ,QNetworkReply *reply )
{
    qDebug() << "OC slotAuthCheck";
    if( reply->error() == QNetworkReply::AuthenticationRequiredError ) { // returned if the user is wrong.
        qDebug() << "******** Password is wrong!";
        QString er = tr("No ownCloud Connection"),
                             tr("<p>Your ownCloud credentials are not correct.</p>"
                                "<p>Please correct them by starting the configuration dialog from the tray!</p>");
        //_actionAddFolder->setEnabled( false );
    } else if( reply->error() == QNetworkReply::OperationCanceledError ) {
        // the username was wrong and ownCloudInfo was closing the request after a couple of auth tries.
        qDebug() << "******** Username or password is wrong!";
        QString er = tr("No ownCloud Connection"),
                             tr("<p>Your ownCloud user name or password is not correct.</p>"
                                "<p>Please correct it by starting the configuration dialog from the tray!</p>");
        //_actionAddFolder->setEnabled( false );
    } else {
        qDebug() << "######## Credentials are ok!";
        d->folderMan->setupFolders();
        //int cnt = d->folderMan->setupFolders();
        d->loadFolders();
    }
}


#include "owncloudsyncdaemon.moc"
