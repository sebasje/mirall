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


#include "directorylister.h"

#include <KGlobalSettings>
#include <kdebug.h>

#include "../applet/owncloud_interface.h"


class DirectoryListerPrivate {
public:
    DirectoryLister *q;
    QDir dir;
    QString path;
    QStringList directories;
    QString filter;
};

DirectoryLister::DirectoryLister(QObject* parent) :
    QObject(parent)
{
    d = new DirectoryListerPrivate;
    d->q = this;
    d->dir = QDir::home();
    init();
}

DirectoryLister::~DirectoryLister()
{
    delete d;
}

void DirectoryLister::init()
{
}

void DirectoryLister::up()
{
    emit directoriesChanged();
}

bool DirectoryLister::exists(const QString &f) const
{
    if (f.isEmpty()) {
        return d->dir.exists();
    } else {
        return QDir(f).exists();
    }
}

QStringList DirectoryLister::directories() const
{
    return d->directories.filter(d->filter);
}

QString DirectoryLister::currentPath() const
{
    return d->dir.absolutePath();
}

void DirectoryLister::enterDirectory(const QString &directory)
{
    //kDebug() << "entering dir: " << directory;
    QString dir = directory;
    if (!dir.startsWith("/")) {
        dir = d->dir.absolutePath() + "/" + directory;
    }
    d->dir.setPath(dir);
    d->directories = d->dir.entryList(QDir::AllDirs);
    //kDebug() << "Dirs: " << d->directories;
    //d->directories.removeAll(QString('.'));
    currentPathChanged();
    emit directoriesChanged();
}

QString DirectoryLister::filter() const
{
    return d->filter;
}

void DirectoryLister::setFilter(const QString &f)
{
    //kDebug() << "Setting filter to : " << f;
    if (f != d->filter) {
        d->filter = f;
        emit directoriesChanged();
        emit filterChanged();
    }
}

QString DirectoryLister::homePath() const
{
    return QDir::homePath();
}

QString DirectoryLister::documentPath() const
{
    return KGlobalSettings::documentPath();
}

QString DirectoryLister::downloadPath() const
{
    return KGlobalSettings::downloadPath();
}

QString DirectoryLister::musicPath() const
{
    return KGlobalSettings::musicPath();
}

QString DirectoryLister::picturesPath() const
{
    return KGlobalSettings::picturesPath();
}

QString DirectoryLister::videosPath() const
{
    return KGlobalSettings::videosPath();
}

#include "directorylister.moc"
