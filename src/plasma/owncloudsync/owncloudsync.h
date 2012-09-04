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


#ifndef OWNCLOUDSYNC_H
#define OWNCLOUDSYNC_H


#include <QObject>
#include <QIcon>
#include <QNetworkReply>
#include <QVariant>
#include <QStringListModel>

class OwncloudSyncPrivate;
namespace Mirall {
    class Folder;
}

class OwncloudSync : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString display READ display NOTIFY displayChanged)

    public:
        explicit OwncloudSync(QObject *parent = 0);
        virtual ~OwncloudSync();

    public Q_SLOTS:
        QString display();
        void refresh();
        QVariantMap folder(QString name);
        QVariantMap folderList();
        void updateFolder(const Mirall::Folder *folder);
        void enableFolder(const QString &name, bool enabled = true);
        void addSyncFolder(const QString &localFolder, const QString &remoteFolder,
                           const QString &alias = QString());
        void removeSyncFolder(const QString &alias);
        void createRemoteFolder(const QString &f);
        void checkRemoteFolder(const QString &f);
        void setupOwncloud(const QString &server, const QString &user, const QString &password);

    Q_SIGNALS:
        void displayChanged();
        void folderListChanged(QVariantMap);
        void folderChanged(QVariantMap);
        void owncloudChanged(QVariantMap);
        void errorChanged(int);
        void errorMessageChanged(QString);
        void statusChanged(int);
        void statusMessageChanged(QString);
        void remoteFolderExists(QString, bool);

    protected Q_SLOTS:
        void slotSyncStateChange(const QString&);
        void slotOwnCloudFound( const QString&, const QString&, const QString&, const QString& );
        void slotNoOwnCloudFound( QNetworkReply* );
        void slotCheckAuthentication();
        void slotAuthCheck( const QString& ,QNetworkReply* );
        void slotCreateRemoteFolderFinished(QNetworkReply::NetworkError);
        void slotDirCheckReply( const QString&, QNetworkReply* );

    private Q_SLOTS:
        void delayedReadConfig();

    private:
        OwncloudSyncPrivate* d;
        void loadFolders();

};

#endif // OWNCLOUDSYNCDAEMON_H
