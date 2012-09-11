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

#ifndef OWNCLOUDFOLDER_H
#define OWNCLOUDFOLDER_H

#include <QDateTime>
#include <QObject>
#include <QString>

class OwncloudFolderPrivate;

class OwncloudFolder : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_ENUMS(Error)

    Q_PROPERTY(int folderStatus READ folderStatus NOTIFY folderStatusChanged)
    Q_PROPERTY(int error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QDateTime syncTime READ syncTime WRITE setSyncTime NOTIFY syncTimeChanged)

    public:
        explicit OwncloudFolder(QObject *parent = 0);
        virtual ~OwncloudFolder();

        enum Status { Disabled, Waiting, Running, Idle, Error };
        enum Error { NoError, TimeSync, Connection, Disk, Other };

        int folderStatus() const;
        int error() const;
        QString errorMessage() const;
        QString name() const;
        QString displayName() const;
        QString localPath() const;
        QString remotePath() const;
        QDateTime syncTime() const;

    public Q_SLOTS:
        void setFolderStatus(int i);
        void setError(int i);
        void setErrorMessage(const QString &n);
        void setName(const QString &n);
        void setDisplayName(const QString &n);
        void setSyncTime(const QDateTime &dt);
        void setLocalPath(const QString &p);
        void setRemotePath(const QString &p);

        Q_INVOKABLE void enable();
        Q_INVOKABLE void disable();
        Q_INVOKABLE void remove();

    Q_SIGNALS:
        void folderStatusChanged();
        void errorChanged();
        void errorMessageChanged();
        void nameChanged();
        void displayNameChanged();
        void localPathChanged();
        void remotePathChanged();
        void syncTimeChanged();
        void enableFolder(const QString &name, bool enabled = true);

    private:
        OwncloudFolderPrivate* d;
        QString statusString(int s);
};

#endif // OWNCLOUDFOLDER_H
