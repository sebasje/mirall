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


#ifndef OWNCLOUDSETTINGS_H
#define OWNCLOUDSETTINGS_H

#include <KIconLoader>

#include "owncloudfolder.h"

#include <QDeclarativeComponent>
#include <QObject>
#include <QIcon>
#include <QVariant>
#include <QStringListModel>


class OwncloudSettingsPrivate;

class OwncloudSettings : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_ENUMS(Error)

    Q_PROPERTY(int owncloudStatus READ owncloudStatus NOTIFY owncloudStatusChanged)
    Q_PROPERTY(int error READ error NOTIFY errorChanged)
    Q_PROPERTY(QDeclarativeListProperty<OwncloudFolder> folders READ folders NOTIFY foldersChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString edition READ edition NOTIFY editionChanged)

    public:
        OwncloudSettings(QObject* parent = 0);
        virtual ~OwncloudSettings();

        enum Status {
            Disconnected,
            Connected,
            Error
        };
        enum Error {
            NoError, // We're fine
            AuthenticationError, // owncloud server doesn't accept credentials
            NetworkError, // server is unreachable
            NoConfigurationError, // no configuration found
            NoDaemonError, // owncloudsyncd is not running
            CustomError // Anything else
        };



    public Q_SLOTS:
        void init();

        QString url() const;
        QString version() const;
        QString edition() const;
        QDeclarativeListProperty<OwncloudFolder> folders();

        void setDisplay(const QString &n);

        int owncloudStatus() const;
        void setOwncloudStatus(int i);

        int error() const;
        void setError(int i);

        QString statusMessage() const;
        void setStatusMessage(const QString &n);

        QString errorMessage() const;
        void setErrorMessage(const QString &n);

        void setFolderList(const QVariantMap &m);
        void setFolder(const QVariantMap &m);
        void setOwncloud(const QVariantMap &m);
        void setUrl(const QString &u);
        void serviceUnregistered();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void startDaemon();
        Q_INVOKABLE void enableFolder(const QString &name, bool enabled = true);

    Q_SIGNALS:
        void dataChanged();
        void owncloudStatusChanged();
        void errorChanged();
        void foldersChanged();
        void statusMessageChanged();
        void errorMessageChanged();
        void editionChanged();
        void versionChanged();
        void urlChanged();

    private:
        OwncloudSettingsPrivate* d;
};

#endif // OWNCLOUDSETTINGS_H
