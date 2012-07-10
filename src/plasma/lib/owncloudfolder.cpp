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

#include "owncloudfolder.h"

#include <kdebug.h>


class OwncloudFolderPrivate {
public:
    OwncloudFolder *q;
    QString name;
};

OwncloudFolder::OwncloudFolder(QObject* parent)
    : QObject(parent)
{
    d = new OwncloudFolderPrivate;
    d->q = this;
    setName("d-fault.");
    setDisplayName(name());
}

OwncloudFolder::~OwncloudFolder()
{
    delete d;
}

QString OwncloudFolder::name()
{
    return d->name;
}

void OwncloudFolder::setName(const QString &n)
{
    d->name = n;
    emit nameChanged();
    setDisplayName(n);
}

QString OwncloudFolder::displayName()
{
    return d->name;
}

void OwncloudFolder::setDisplayName(const QString &n)
{
    if (d->name == n) {
        return;
    }
    d->name = n;
    emit displayNameChanged();
}

#include "owncloudfolder.moc"
