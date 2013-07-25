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

#ifndef SYNCPROGRESS_H
#define SYNCPROGRESS_H

#include <QObject>
#include <QString>
#include <QVariant>

class SyncProgress : public QObject
{
    Q_OBJECT

    Q_ENUMS(Kind)

    Q_PROPERTY(qint64 current READ current NOTIFY currentChanged)
    Q_PROPERTY(qint64 total READ total NOTIFY totalChanged)
    Q_PROPERTY(QString folder READ folder NOTIFY folderChanged)
    Q_PROPERTY(QString file READ file NOTIFY fileChanged)

    public:
        explicit SyncProgress(QObject *parent = 0);
        virtual ~SyncProgress();

        enum Kind { Disabled, Waiting, Running, Idle, Error };

        qint64 current() const;
        qint64 total() const;
        QString folder() const;
        QString file() const;

    Q_SIGNALS:
        void currentChanged();
        void totalChanged();
        void folderChanged();
        void fileChanged();
        void progressChanged();

    public Q_SLOTS:
        void setProgress(const QVariantMap &m);

    private:
        QVariantMap d;
};

#endif // SYNCPROGRESS_H
