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

// Own
#include "owncloudconfig.h"

// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtDBus/QtDBus>

// KDE
#include <kconfiggroup.h>
#include <kdebug.h>
#include <kfileitem.h>
#include <kglobalsettings.h>
#include <kio/copyjob.h>
#include <kio/deletejob.h>
#include <kio/job.h>
#include <kio/jobuidelegate.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kmimetype.h>
#include <kstandarddirs.h>
#include <kurlrequester.h>

#include "kcmowncloud.h"
#include "ui_owncloudconfig.h"


class OwncloudConfigPrivate {
public:
    Ui_OwncloudConfig* ui;

};


//-----------------------------------------------------------------------------

OwncloudConfig::OwncloudConfig(QWidget *parent, const QVariantList &)
    : KCModule( KcmOwncloud::componentData(), parent )
{
    d = new OwncloudConfigPrivate;
    d->ui = new Ui_OwncloudConfig();
    d->ui->setupUi(this);
/*    
  QBoxLayout *lay = new QBoxLayout(this);
  lay->setMargin(0);*/

  setQuickHelp( i18n("<h1>Owncloud</h1>\n"
    "This module allows you to configure ownCloud servers and folders you want to synchronize."));

}

void OwncloudConfig::load()
{
    kDebug() << "Load settings....";
    emit changed(true);
}

void OwncloudConfig::defaults()
{
    kDebug() << "loading defaults....";

}


void OwncloudConfig::save()
{
    kDebug() << "Save settings....";
}


#include "owncloudconfig.moc"
