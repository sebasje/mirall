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


#ifndef OWNCLOUDSYNCDAEMON_H
#define OWNCLOUDSYNCDAEMON_H


#include <QObject>
#include <QIcon>
#include <QVariant>
#include <QStringListModel>

class OwncloudSyncDaemonPrivate;

class OwncloudSyncDaemon : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString display READ display NOTIFY displayChanged)

    public:
        explicit OwncloudSyncDaemon(QObject *parent = 0);
        virtual ~OwncloudSyncDaemon();

    public Q_SLOTS:
        QString display();
        QVariantMap folder(QString name);
        QVariantMap folderList();

        void timeout();

    Q_SIGNALS:
        void displayChanged();
        void folderListChanged(QVariantMap);
        void statusMessageChanged(QString);

    private:
        OwncloudSyncDaemonPrivate* d;
};

#endif // OWNCLOUDSYNCDAEMON_H
