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

#ifndef OWNCLOUDAPPLET_H
#define OWNCLOUDAPPLET_H

#include <Plasma/PopupApplet>
#include <Plasma/Label>

#include <QQmlComponent>

namespace Plasma {
    class DeclarativeWidget;
}

class OwncloudApplet : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        OwncloudApplet(QObject *parent, const QVariantList &args);
        ~OwncloudApplet();

        void init();
        QQuickItem *graphicsWidget();

    public Q_SLOTS:
        void configChanged();

    protected:
        void createConfigurationInterface(KConfigDialog *parent);

    protected Q_SLOTS:
        void configAccepted();
        void connectObjects(QQmlComponent::Status);
        void connectObjects();
        void updatePopupIcon(int i);

    private:
        Plasma::DeclarativeWidget *m_declarativeWidget;
        //Plasma::Package *m_package;
};

K_EXPORT_PLASMA_APPLET(owncloud, OwncloudApplet)

#endif
