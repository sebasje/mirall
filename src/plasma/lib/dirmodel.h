/***************************************************************************
 *   Copyright 2012 by Sebastian Kügler <sebas@kde.org>                    *
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

#ifndef DIRMODEL_H
#define DIRMODEL_H

#include <QAbstractListModel>

#include <KFileItem>
#include <KDirModel>
#include <QDir>


class DirModelPrivate;

class DirModel : public KDirModel
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString mimeFilter READ mimeFilter WRITE setMimeFilter NOTIFY mimeFilterChanged)

public:
    enum FileItemRoles {
        NameRole = Qt::UserRole + 1,
        IconNameRole,
        CommentRole,
        IsDirRole,
        IsFileRole,
        IsLinkRole,
        IsLocalFileRole,
        MimetypeRole,
        ModificationTimeRole,
        AccessTimeRole,
        CreationTimeRole,
        UrlRole
    };

    DirModel(QObject *parent = 0);
    virtual ~DirModel();

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString url() const;
    void setUrl(const QString &u);

    QString mimeFilter() const;
    void setMimeFilter(const QString &m);

Q_SIGNALS:
    void urlChanged();
    void mimeFilterChanged();

private:
    DirModelPrivate *d;
};

#endif // DIRMODEL_H
