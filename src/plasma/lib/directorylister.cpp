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

#include "../applet/owncloud_interface.h"

#include <kdebug.h>

class DirectoryListerPrivate {
public:
    DirectoryLister *q;
    QDir dir;
    QString path;
    QStringList directories;
};

DirectoryLister::DirectoryLister(QObject* parent) :
    QObject(parent)
{
    d = new DirectoryListerPrivate;
    d->q = this;

    //d->directories = QStringList() << "/home/sebas/tmp/test1/" << "/home/sebas/tmp/test2/" << "/home/sebas/tmp/test3/" << "/home/sebas/tmp/test4/" << "/home/sebas/tmp/test5/";

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

QStringList DirectoryLister::directories()
{
    return d->directories;
}

void DirectoryLister::enterDirectory(const QString &directory)
{
    kDebug() << "entering dir: " << directory;
    QString dir = directory;
    if (!dir.startsWith("/")) {
        dir = d->dir.path() + "/" + directory;
    }
    d->dir.setPath(dir);
    d->directories = d->dir.entryList(QDir::AllDirs);
    kDebug() << "Dirs: " << d->directories;
    //d->directories.removeAll(QString('.'));
    emit directoriesChanged();
}


#include "directorylister.moc"
