/*
 * Copyright (C) by Duncan Mac-Vicar P. <duncan@kde.org>
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include "mirall/csyncthread.h"
#include "mirall/mirallconfigfile.h"
#include "mirall/theme.h"
#include "mirall/logger.h"
#include "mirall/utility.h"

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <QDebug>
#include <QDir>
#include <QMutexLocker>
#include <QThread>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <QApplication>
#include <QUrl>
#include <QSslCertificate>

namespace Mirall {

/* static variables to hold the credentials */
QString CSyncThread::_user;
QString CSyncThread::_passwd;
QNetworkProxy CSyncThread::_proxy;

QString CSyncThread::_csyncConfigDir;  // to be able to remove the lock file.

QMutex CSyncThread::_mutex;

void csyncLogCatcher(CSYNC *ctx,
                     int verbosity,
                     const char *function,
                     const char *buffer,
                     void *userdata)
{
  Logger::instance()->csyncLog( QString::fromUtf8(buffer) );
}

CSyncThread::CSyncThread(const QString &source, const QString &target)
    : _source(source)
    , _target(target)

{
    _mutex.lock();
    if( ! _source.endsWith(QLatin1Char('/'))) _source.append(QLatin1Char('/'));
    _mutex.unlock();
}

CSyncThread::~CSyncThread()
{

}

QString CSyncThread::csyncErrorToString( CSYNC_ERROR_CODE err, const char *errString )
{
    QString errStr;

    switch( err ) {
    case CSYNC_ERR_NONE:
        errStr = tr("Success.");
        break;
    case CSYNC_ERR_LOG:
        errStr = tr("CSync Logging setup failed.");
        break;
    case CSYNC_ERR_LOCK:
        errStr = tr("CSync failed to create a lock file.");
        break;
    case CSYNC_ERR_STATEDB_LOAD:
        errStr = tr("CSync failed to load the state db.");
        break;
    case CSYNC_ERR_MODULE:
        errStr = tr("<p>The %1 plugin for csync could not be loaded.<br/>Please verify the installation!</p>").arg(Theme::instance()->appName());
        break;
    case CSYNC_ERR_TIMESKEW:
        errStr = tr("The system time on this client is different than the system time on the server. "
                    "Please use a time synchronization service (NTP) on the server and client machines "
                    "so that the times remain the same.");
        break;
    case CSYNC_ERR_FILESYSTEM:
        errStr = tr("CSync could not detect the filesystem type.");
        break;
    case CSYNC_ERR_TREE:
        errStr = tr("CSync got an error while processing internal trees.");
        break;
    case CSYNC_ERR_MEM:
        errStr = tr("CSync failed to reserve memory.");
        break;
    case CSYNC_ERR_PARAM:
        errStr = tr("CSync fatal parameter error.");
        break;
    case CSYNC_ERR_UPDATE:
        errStr = tr("CSync processing step update failed.");
        break;
    case CSYNC_ERR_RECONCILE:
        errStr = tr("CSync processing step reconcile failed.");
        break;
    case CSYNC_ERR_PROPAGATE:
        errStr = tr("CSync processing step propagate failed.");
        break;
    case CSYNC_ERR_ACCESS_FAILED:
        errStr = tr("<p>The target directory %1 does not exist.</p><p>Please check the sync setup.</p>").arg(_target);
        // this is critical. The database has to be removed.
        emit wipeDb();
        break;
    case CSYNC_ERR_REMOTE_CREATE:
    case CSYNC_ERR_REMOTE_STAT:
        errStr = tr("A remote file can not be written. Please check the remote access.");
        break;
    case CSYNC_ERR_LOCAL_CREATE:
    case CSYNC_ERR_LOCAL_STAT:
        errStr = tr("The local filesystem can not be written. Please check permissions.");
        break;
    case CSYNC_ERR_PROXY:
        errStr = tr("CSync failed to connect through a proxy.");
        break;
    case CSYNC_ERR_LOOKUP:
        errStr = tr("CSync failed to lookup proxy or server.");
        break;
    case CSYNC_ERR_AUTH_SERVER:
        errStr = tr("CSync failed to authenticate at the %1 server.").arg(Theme::instance()->appName());
        break;
    case CSYNC_ERR_AUTH_PROXY:
        errStr = tr("CSync failed to authenticate at the proxy.");
        break;
    case CSYNC_ERR_CONNECT:
        errStr = tr("CSync failed to connect to the network.");
        break;
    case CSYNC_ERR_TIMEOUT:
        errStr = tr("A network connection timeout happend.");
        break;
    case CSYNC_ERR_HTTP:
        errStr = tr("A HTTP transmission error happened.");
        break;
    case CSYNC_ERR_PERM:
        errStr = tr("CSync failed due to not handled permission deniend.");
        break;
    case CSYNC_ERR_NOT_FOUND:
        errStr = tr("CSync failed to find a specific file.");
        break;
    case CSYNC_ERR_EXISTS:
        errStr = tr("CSync tried to create a directory that already exists.");
        break;
    case CSYNC_ERR_NOSPC:
        errStr = tr("CSync: No space on %1 server available.").arg(Theme::instance()->appName());
        break;
    case CSYNC_ERR_UNSPEC:
        errStr = tr("CSync unspecified error.");

    default:
        errStr = tr("An internal error number %1 happend.").arg( (int) err );
    }

    if( errString ) {
        errStr += tr("<br/>Backend Message: ")+QString::fromUtf8(errString);
    }
    return errStr;

}

const char* CSyncThread::proxyTypeToCStr(QNetworkProxy::ProxyType type)
{
    switch (type) {
    case QNetworkProxy::NoProxy:
        return "NoProxy";
    case QNetworkProxy::DefaultProxy:
        return "DefaultProxy";
    case QNetworkProxy::Socks5Proxy:
        return "Socks5Proxy";
    case  QNetworkProxy::HttpProxy:
        return "HttpProxy";
    case  QNetworkProxy::HttpCachingProxy:
        return "HttpCachingProxy";
    case  QNetworkProxy::FtpCachingProxy:
        return "FtpCachingProxy";
    default:
        return "NoProxy";
    }
}

void CSyncThread::startSync()
{
    qDebug() << "starting to sync " << qApp->thread() << QThread::currentThread();
    CSYNC *csync;

    emit(started());

    _mutex.lock();

    if( csync_create(&csync,
                     _source.toUtf8().data(),
                     _target.toUtf8().data()) < 0 ) {
        emit csyncError( tr("CSync create failed.") );
    }
    _csyncConfigDir = QString::fromUtf8( csync_get_config_dir( csync ));
    _mutex.unlock();

    csync_enable_conflictcopys(csync);

    MirallConfigFile cfg;
    QString excludeList = cfg.excludeFile();

    if( !excludeList.isEmpty() ) {
        qDebug() << "==== added CSync exclude List: " << excludeList.toUtf8();
        csync_add_exclude_list( csync, excludeList.toUtf8() );
    }

    csync_set_config_dir( csync, cfg.configPath().toUtf8() );

    QTime t;
    t.start();

    csync_set_userdata(csync, this);

    csync_set_log_callback( csync, csyncLogCatcher );
    csync_set_auth_callback( csync, getauth );
    csync_set_progress_callback( csync, progress );

    if( csync_init(csync) < 0 ) {
        CSYNC_ERROR_CODE err = csync_get_error( csync );
        const char *errMsg = csync_get_error_string( csync );
        QString errStr = csyncErrorToString(err, errMsg );
        qDebug() << " #### ERROR csync_init: " << errStr;
        emit csyncError(errStr);
        goto cleanup;
    }

    // set module properties, mainly the proxy information.
    // do not use QLatin1String here because that has to be real const char* for C.
    csync_set_log_verbosity(csync, 11);
    csync_set_module_property(csync, "csync_context", csync);
    csync_set_module_property(csync, "proxy_type", (char*) proxyTypeToCStr(_proxy.type()) );
    csync_set_module_property(csync, "proxy_host", _proxy.hostName().toUtf8().data() );
    csync_set_module_property(csync, "proxy_user", _proxy.user().toUtf8().data()     );
    csync_set_module_property(csync, "proxy_pwd" , _proxy.password().toUtf8().data() );

    qDebug() << "#### Update start #################################################### >>";
    if( csync_update(csync) < 0 ) {
        CSYNC_ERROR_CODE err = csync_get_error( csync );
        const char *errMsg = csync_get_error_string( csync );
        QString errStr = csyncErrorToString(err, errMsg);
        qDebug() << " #### ERROR csync_update: " << errStr;
        emit csyncError(errStr);
        goto cleanup;
    }
    qDebug() << "<<#### Update end ###########################################################";

    if( csync_reconcile(csync) < 0 ) {
        CSYNC_ERROR_CODE err = csync_get_error( csync );
        const char *errMsg = csync_get_error_string( csync );
        QString errStr = csyncErrorToString(err, errMsg);
        qDebug() << " #### ERROR csync_reconcile: " << errStr;
        emit csyncError(errStr);
        goto cleanup;
    }
    if( csync_propagate(csync) < 0 ) {
        CSYNC_ERROR_CODE err = csync_get_error( csync );
        const char *errMsg = csync_get_error_string( csync );
        QString errStr = csyncErrorToString(err, errMsg);
        qDebug() << " #### ERROR csync_propagate: " << errStr;
        emit csyncError(errStr);
        goto cleanup;
    }
cleanup:
    csync_destroy(csync);

    qDebug() << "CSync run took " << t.elapsed() << " Milliseconds";
    emit(finished());
}

void CSyncThread::setConnectionDetails( const QString &user, const QString &passwd, const QNetworkProxy &proxy )
{
    _mutex.lock();
    _user = user;
    _passwd = passwd;
    _proxy = proxy;
    _mutex.unlock();
}

QString CSyncThread::csyncConfigDir()
{
    return _csyncConfigDir;
}

int CSyncThread::getauth(const char *prompt,
                         char *buf,
                         size_t len,
                         int echo,
                         int verify,
                         void *userdata
                         )
{
    int re = 0;

    QString qPrompt = QString::fromLatin1( prompt ).trimmed();

    if( qPrompt == QLatin1String("Enter your username:") ) {
        // qDebug() << "OOO Username requested!";
        QMutexLocker locker( &_mutex );
        qstrncpy( buf, _user.toUtf8().constData(), len );
    } else if( qPrompt == QLatin1String("Enter your password:") ) {
        QMutexLocker locker( &_mutex );
        // qDebug() << "OOO Password requested!";
        qstrncpy( buf, _passwd.toUtf8().constData(), len );
    } else {
        if( qPrompt.startsWith( QLatin1String("There are problems with the SSL certificate:"))) {
            // SSL is requested. If the program came here, the SSL check was done by mirall
            // the answer is simply yes here.
            QRegExp regexp("fingerprint: ([\\w\\d:]+)");
            bool certOk = false;

            int pos = 0;
            MirallConfigFile cfg;
            QByteArray ba = cfg.caCerts();

            QList<QSslCertificate> certs = QSslCertificate::fromData(ba);
            Utility util;

            while (!certOk && (pos = regexp.indexIn(qPrompt, 1+pos)) != -1) {
                QString neon_fingerprint = regexp.cap(1);

                foreach( const QSslCertificate& c, certs ) {
                    QString verified_shasum = util.formatFingerprint(c.digest(QCryptographicHash::Sha1).toHex());
                    qDebug() << "SSL Fingerprint from neon: " << neon_fingerprint << " compared to verified: " << verified_shasum;
                    if( verified_shasum == neon_fingerprint ) {
                        certOk = true;
                        break;
                    }
                }
            }
            // certOk = false;     DEBUG setting, keep disabled!
            if( !certOk ) { // Problem!
                qstrcpy( buf, "no" );
                re = -1;
            } else {
                qstrcpy( buf, "yes" ); // Certificate is fine!
            }
        } else {
            qDebug() << "Unknown prompt: <" << prompt << ">";
            re = -1;
        }
    }
    return re;
}


void CSyncThread::progress(const char *remote_url, enum csync_notify_type_e kind,
                                        long long o1, long long o2, void *userdata)
{
    (void) o1; (void) o2;
    if (kind == CSYNC_NOTIFY_FINISHED_DOWNLOAD) {
        QString path = QUrl::fromEncoded(remote_url).toString();
        CSyncThread *thread = static_cast<CSyncThread*>(userdata);
        thread->fileReceived(path);
    }
}


}
