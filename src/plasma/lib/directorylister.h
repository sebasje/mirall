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


#ifndef DIRECTORYLISTER_H
#define DIRECTORYLISTER_H

#include <QFile>
#include <QDir>

class DirectoryListerPrivate;

class DirectoryLister : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList directories READ directories NOTIFY directoriesChanged)
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

    Q_PROPERTY(QString homePath READ homePath NOTIFY homePathChanged)
    Q_PROPERTY(QString documentPath READ documentPath NOTIFY documentPathChanged)
    Q_PROPERTY(QString downloadPath READ downloadPath NOTIFY downloadPathChanged)
    Q_PROPERTY(QString musicPath READ musicPath NOTIFY musicPathChanged)
    Q_PROPERTY(QString picturesPath READ picturesPath NOTIFY picturesPathChanged)
    Q_PROPERTY(QString videosPath READ videosPath NOTIFY videosPathChanged)

    public:
        DirectoryLister(QObject* parent = 0);
        ~DirectoryLister();


    public Q_SLOTS:
        QStringList directories() const;
        QString currentPath() const;
        QString filter() const;

        QString homePath() const;
        QString documentPath() const;
        QString downloadPath() const;
        QString musicPath() const;
        QString picturesPath() const;
        QString videosPath() const;

        void setFilter(const QString &f);

        Q_INVOKABLE void up();
        Q_INVOKABLE void enterDirectory(const QString &directory);
        Q_INVOKABLE bool exists(const QString &f = QString()) const;

    Q_SIGNALS:
        void directoriesChanged();
        void currentPathChanged();
        void filterChanged();

        void homePathChanged();
        void documentPathChanged();
        void downloadPathChanged();
        void musicPathChanged();
        void picturesPathChanged();
        void videosPathChanged();

    private:
        DirectoryListerPrivate* d;
};

#endif // DIRECTORYLISTER_H
