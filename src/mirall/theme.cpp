/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include "theme.h"
#include "version.h"
#include "config.h"

#include <QtCore>
#include <QtGui>

#include "mirall/owncloudtheme.h"

#ifdef THEME_INCLUDE
#  define QUOTEME(M)       #M
#  define INCLUDE_FILE(M)  QUOTEME(M)
#  include INCLUDE_FILE(THEME_INCLUDE)
#endif

namespace Mirall {

Theme* Theme::_instance = 0;

Theme* Theme::instance() {
    if (!_instance) {
        _instance = new THEME_CLASS;
        _instance->_mono = false;
    }
    return _instance;
}

QString Theme::statusHeaderText( SyncResult::Status status ) const
{
    QString resultStr;

    switch( status ) {
    case SyncResult::Undefined:
        resultStr = QObject::tr("Status undefined");
        break;
    case SyncResult::NotYetStarted:
        resultStr = QObject::tr("Waiting to start sync");
        break;
    case SyncResult::SyncRunning:
        resultStr = QObject::tr("Sync is running");
        break;
    case SyncResult::Success:
        resultStr = QObject::tr("Sync Success");
        break;
    case SyncResult::Error:
        resultStr = QObject::tr("Sync Error - Click info button for details.");
        break;
    case SyncResult::SetupError:
        resultStr = QObject::tr( "Setup Error" );
        break;
    default:
        resultStr = QObject::tr("Status undefined");
    }
    return resultStr;
}

QString Theme::appNameGUI() const
{
    return appName();
}

QString Theme::version() const
{
    return QString::fromLocal8Bit( MIRALL_STRINGIFY( MIRALL_VERSION ));
}

QIcon Theme::trayFolderIcon( const QString& backend ) const
{
    Q_UNUSED(backend)
    return applicationIcon();
}

/*
 * helper to load a icon from either the icon theme the desktop provides or from
 * the apps Qt resources.
 */
QIcon Theme::themeIcon( const QString& name, bool sysTray ) const
{
    QString flavor;
    if (sysTray && _mono) {
#ifdef Q_OS_MAC
        flavor = QLatin1String("black");
#else
        flavor = QLatin1String("white");
#endif
    } else {
        flavor = QLatin1String("colored");
    }

    QIcon icon;
    if( QIcon::hasThemeIcon( name )) {
        // use from theme
        icon = QIcon::fromTheme( name );
    } else {
        QList<int> sizes;
        sizes <<16 << 22 << 32 << 48 << 64 << 128;
        foreach (int size, sizes) {
            QString pixmapName = QString::fromLatin1(":/mirall/theme/%1/%2-%3.png").arg(flavor).arg(name).arg(size);
            if (QFile::exists(pixmapName)) {
                QPixmap px(pixmapName);
                // HACK, get rid of it by supporting FDO icon themes, this is really just emulating ubuntu-mono
                if (qgetenv("DESKTOP_SESSION") == "ubuntu") {
                    QBitmap mask = px.createMaskFromColor(Qt::white, Qt::MaskOutColor);
                    QPainter p(&px);
                    p.setPen(QColor("#dfdbd2"));
                    p.drawPixmap(px.rect(), mask, mask.rect());
                }
                icon.addPixmap(px);
            }
        }
        if (icon.isNull()) {
            foreach (int size, sizes) {
                QString pixmapName = QString::fromLatin1(":/mirall/resources/%1-%2.png").arg(name).arg(size);
                if (QFile::exists(pixmapName)) {
                    icon.addFile(pixmapName);
                }
            }
        }
    }
    return icon;
}

// if this option return true, the client only supports one folder to sync.
// The Add-Button is removed accoringly.
bool Theme::singleSyncFolder() const {
    return false;
}

QString Theme::defaultServerFolder() const
{
    return QLatin1String("/");
}

QString Theme::overrideServerUrl() const
{
    return QString::null;
}

QString Theme::defaultClientFolder() const
{
    return appName();
}

void Theme::setSystrayUseMonoIcons(bool mono)
{
    _mono = mono;
    emit systrayUseMonoIconsChanged(mono);
}

bool Theme::systrayUseMonoIcons() const
{
    return _mono;
}

QString Theme::about() const
{
    return QString::null;
}

QVariant Theme::customMedia( CustomMediaType type )
{
    QVariant re;
    QString key;

    switch ( type )
    {
    case oCSetupTop:
        key = QLatin1String("oCSetupTop");
        break;
    case oCSetupSide:
        key = QLatin1String("oCSetupSide");
        break;
    case oCSetupBottom:
        key = QLatin1String("oCSetupBottom");
        break;
    case oCSetupResultTop:
        key = QLatin1String("oCSetupResultTop");
        break;
    }

    QString imgPath = QString::fromLatin1(":/mirall/theme/colored/%1.png").arg(key);
    if ( QFile::exists( imgPath ) ) {
        QPixmap pix( imgPath );
        if( pix.isNull() ) {
            // pixmap loading hasn't succeeded. We take the text instead.
            re.setValue( key );
        } else {
            re.setValue( pix );
        }
    }
    return re;
}

QIcon Theme::syncStateIcon( SyncResult::Status status, bool sysTray ) const
{
    // FIXME: Mind the size!
    QString statusIcon;

    switch( status ) {
    case SyncResult::Undefined:
    case SyncResult::NotYetStarted:
    case SyncResult::Unavailable:
        statusIcon = QLatin1String("state-offline");
        break;
    case SyncResult::SyncRunning:
        statusIcon = QLatin1String("state-sync");
        break;
    case SyncResult::SyncPrepare:
    case SyncResult::Success:
        statusIcon = QLatin1String("state-ok");
        break;
    case SyncResult::Error:
    case SyncResult::SetupError:
    default:
        statusIcon = QLatin1String("state-error");
    }

    return themeIcon( statusIcon, sysTray );
}

QColor Theme::wizardHeaderTitleColor() const
{
    return qApp->palette().text().color();
}

QColor Theme::wizardHeaderBackgroundColor() const
{
    return QColor();
}

QPixmap Theme::wizardHeaderLogo() const
{
    return applicationIcon().pixmap(64);
}

QPixmap Theme::wizardHeaderBanner() const
{
    QColor c = wizardHeaderBackgroundColor();
    if (!c.isValid())
        return QPixmap();

    QPixmap pix(QSize(600, 78));
    pix.fill(wizardHeaderBackgroundColor());
    return pix;
}

} // end namespace mirall

