/***************************************************************************
 *                                                                         *
 *   Copyright 2012,2015 Sebastian Kügler <sebas@kde.org>                  *
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
 *                                                                         *
 ***************************************************************************/


#ifndef OWNCLOUDSETTINGS_H
#define OWNCLOUDSETTINGS_H


#include "owncloudfolder.h"
#include "syncprogress.h"

#include <QQmlListProperty>
#include <QQmlComponent>
#include <QObject>
#include <QIcon>
#include <QVariant>
#include <QStringListModel>

class MiniJob;
class CreateFolderJob;

class OwncloudSettingsPrivate;

class OwncloudSettings : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_ENUMS(Error)

    Q_PROPERTY(int owncloudStatus READ owncloudStatus NOTIFY owncloudStatusChanged)
    Q_PROPERTY(int error READ error NOTIFY errorChanged)
    Q_PROPERTY(QQmlListProperty<OwncloudFolder> folders READ folders NOTIFY foldersChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(SyncProgress* progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int globalStatus READ globalStatus NOTIFY globalStatusChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString edition READ edition NOTIFY editionChanged)

    public:
        OwncloudSettings(QObject* parent = 0);
        virtual ~OwncloudSettings();

        enum Status {
            Disconnected = 0,
            Connected,
            Error
        };
        enum Error {
            NoError = 0, // We're fine
            AuthenticationError, // owncloud server doesn't accept credentials
            NetworkError, // server is unreachable
            NoConfigurationError, // no configuration found
            NoDaemonError, // owncloudsyncd is not running
            CustomError // Anything else
        };

        static QString statusString(int s);

    public Q_SLOTS:
        void init();

        QString url() const;
        QString version() const;
        QString edition() const;
        int globalStatus() const; // Returns OwncloudFolder::Status
        QQmlListProperty<OwncloudFolder> folders();

        int owncloudStatus() const; // returns OwncloudSettings::Status
        void setOwncloudStatus(int i);

        int error() const;
        void setError(int i);

        void setFolderList(const QVariantMap &m);
        void setFolder(const QVariantMap &m);
        void setOwncloud(const QVariantMap &m);
        void setProgress(const QVariantMap &m);
        SyncProgress* progress() const;
        void setUrl(const QString &u);
        void serviceUnregistered();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void enableAllFolders(bool enabled);
        Q_INVOKABLE void startDaemon();
        Q_INVOKABLE void enableFolder(const QString &name, bool enabled = true);
        Q_INVOKABLE void addSyncFolder(const QString &localFolder, const QString &remoteFolder,
                                       const QString &aliasName);
        Q_INVOKABLE void removeSyncFolder(const QString &alias);
        Q_INVOKABLE void checkRemoteFolder(const QString &folder);
        Q_INVOKABLE CreateFolderJob* createRemoteFolder(const QString &folder);
        Q_INVOKABLE bool createLocalFolder(const QString &folder);
        Q_INVOKABLE QString verifyFolder(const QString &localFolder, const QString &remoteFolder,
                                       const QString &alias);
        Q_INVOKABLE bool isConfigured(const QString &localFolder, const QString &remoteFolder,
                                       const QString &alias);
        Q_INVOKABLE void setupOwncloud(const QString &server, const QString &user,
                                       const QString &password);
        Q_INVOKABLE MiniJob* createMiniJob();
        Q_INVOKABLE void openConfig();

    Q_SIGNALS:
        void dataChanged();
        void owncloudStatusChanged();
        void errorChanged();
        void foldersChanged();
        void globalStatusChanged();
        void statusMessageChanged();
        void progressChanged();
        void errorMessageChanged();
        void editionChanged();
        void versionChanged();
        void urlChanged();
        void remoteFolderExists(const QString &folder, bool exists);
        //void folderVerified(const QString &folder, const QString &error);

    private Q_SLOTS:
        void updateGlobalStatus();
        void slotRemoteFolderExists(const QString &folder, bool exists);
        //void slotCreateRemoteFolderFinished();

    private:
        OwncloudSettingsPrivate* d;
        QString errorString(int e);

};

#endif // OWNCLOUDSETTINGS_H
