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

    Q_PROPERTY(QDeclarativeListProperty<OwncloudFolder> folders READ folders NOTIFY foldersChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString edition READ edition NOTIFY editionChanged)

    public:
        OwncloudSettings(QObject* parent = 0);
        virtual ~OwncloudSettings();

        void init();

    public Q_SLOTS:
        QDeclarativeListProperty<OwncloudFolder> folders();

        QString url();
        QString version();
        QString edition();
        void setDisplay(const QString &n);

        QString statusMessage();
        void setStatusMessage(const QString &n);

        void setFolderList(const QVariantMap &m);
        void setFolder(const QVariantMap &m);
        void setOwncloud(const QVariantMap &m);
        void setUrl(const QString &u);

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void enableFolder(const QString &name, bool enabled = true);

    Q_SIGNALS:
        void dataChanged();
        void foldersChanged();
        void statusMessageChanged();
        void editionChanged();
        void versionChanged();
        void urlChanged();

    private:
        OwncloudSettingsPrivate* d;
};

#endif // OWNCLOUDSETTINGS_H
