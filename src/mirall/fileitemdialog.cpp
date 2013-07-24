/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include <QtGui>

#include "mirall/fileitemdialog.h"
#include "mirall/syncresult.h"
#include "mirall/logger.h"

#include "ui_fileitemdialog.h"

#define TYPE_SUCCESS  1
#define TYPE_CONFLICT 2
#define TYPE_NEW      3
#define TYPE_DELETED  4
#define TYPE_ERROR    5
#define TYPE_RENAME   6
#define TYPE_IGNORE   7

#define FILE_TYPE    100

namespace Mirall {

FileItemDialog::FileItemDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::FileItemDialog)
{
    _ui->setupUi(this);
    connect(_ui->_dialogButtonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()),
            this, SLOT(accept()));

    QStringList header;
    header << tr("Files");
    QString firstColString = tr("File Count");
    header << firstColString;
    _ui->_treeWidget->setHeaderLabels( header );

    _ui->_treeWidget->setColumnWidth(0, 480);
    _timer.setInterval(1000);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(slotSetFolderMessage()));
    connect(this, SIGNAL(guiLog(QString,QString)), Logger::instance(), SIGNAL(guiLog(QString,QString)));

    QPushButton *copyBtn = _ui->_dialogButtonBox->addButton(tr("Copy"), QDialogButtonBox::ActionRole);
    connect(copyBtn, SIGNAL(clicked()), SLOT(copyToClipboard()));

    setWindowTitle(tr("Sync Protocol"));

}

FileItemDialog::~FileItemDialog()
{
    delete _ui;
}

void FileItemDialog::setSyncResult( const SyncResult& result )
{
    QString folderMessage;

    SyncResult::Status syncStatus = result.status();
    switch( syncStatus ) {
    case SyncResult::Undefined:
        folderMessage = tr( "Undefined Folder State" );
        break;
    case SyncResult::NotYetStarted:
        folderMessage = tr( "The folder waits to start syncing." );
        break;
    case SyncResult::SyncPrepare:
        folderMessage = tr( "Determining which files to sync." );
        break;
    case SyncResult::Unavailable:
        folderMessage = tr( "Server is currently not available." );
        break;
    case SyncResult::SyncRunning:
        folderMessage = tr("Sync is running.");
        break;
    case SyncResult::Success:
        folderMessage = tr("Last Sync was successful.");
        break;
    case SyncResult::Error:
        folderMessage = tr( "Syncing Error." );
        break;
    case SyncResult::SetupError:
        folderMessage = tr( "Setup Error." );
        break;
    default:
        folderMessage = tr( "Undefined Error State." );
    }

    _folderMessage = folderMessage;
    _lastSyncTime = result.syncTime();

    if( result.errorStrings().count() ) {
        _ui->_errorLabel->setVisible(true);
        _ui->_errorLabel->setTextFormat(Qt::RichText);
        QString errStr;
        foreach( QString err, result.errorStrings() ) {
            errStr.append(QString("<p>%1</p>").arg(err));
        }

        _ui->_errorLabel->setText(errStr);
    } else {
        _ui->_errorLabel->setText(QString::null);
        _ui->_errorLabel->setVisible(false);
    }

    slotSetFolderMessage();
    if( syncStatus == SyncResult::SyncRunning ) {
        _timer.stop();
    } else {
        _timer.start();
    }

    setSyncFileItems( result.syncFileItemVector() );

}

void FileItemDialog::slotSetFolderMessage()
{
    QDateTime now = QDateTime::currentDateTime();
    int secs = _lastSyncTime.secsTo(now);

    if (secs < 60)
        _ui->_timelabel->setText(tr("%1 (last finished %n sec. ago)", "", secs).arg(_folderMessage));
    else
        _ui->_timelabel->setText(tr("%1 (last finished %n min. ago)", "", secs/60).arg(_folderMessage));
}

void FileItemDialog::copyToClipboard()
{
    QString text;
    QTextStream ts(&text);

    int topLevelItems = _ui->_treeWidget->topLevelItemCount();
    for (int i = 0; i < topLevelItems; i++) {
        QTreeWidgetItem *item = _ui->_treeWidget->topLevelItem(i);
        ts << left << qSetFieldWidth(50)
           << item->data(0, Qt::DisplayRole).toString()
           << right << qSetFieldWidth(6)
           << item->data(1, Qt::DisplayRole).toString()
           << endl;
        int childItems = item->childCount();
        for (int j = 0; j < childItems; j++) {
            QTreeWidgetItem *child =item->child(j);
            ts << left << qSetFieldWidth(0) << QLatin1String("   ")
               << child->data(0,Qt::DisplayRole).toString()
               << QString::fromLatin1(" (%1)").arg(
                      child->data(1, Qt::DisplayRole).toString()
                      )
               << endl;
        }
    }

    QApplication::clipboard()->setText(text);
    emit guiLog(tr("Copied to clipboard"), tr("The sync protocol has been copied to the clipboard."));
}

void FileItemDialog::accept()
{
    _timer.stop();
    QDialog::accept();
}

void FileItemDialog::setSyncFileItems( const SyncFileItemVector& list )
{
    _ui->_treeWidget->clear();
    QStringList strings;
    QFont headerFont;
    headerFont.setWeight(QFont::Bold);

    strings.clear();
    strings.append(tr("Synced Files"));
    _syncedFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_SUCCESS );
    _syncedFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_syncedFileItem);

    strings.clear();
    strings.append(tr("New Files"));
    _newFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_NEW );
    _newFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_newFileItem);

    strings.clear();
    strings.append(tr("Deleted Files"));
    _deletedFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_DELETED );
    _deletedFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_deletedFileItem);

    strings.clear();
    strings.append(tr("Renamed Files"));
    _renamedFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_RENAME);
    _renamedFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_renamedFileItem);

    strings.clear();
    strings.append(tr("Ignored Files"));
    _ignoredFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_IGNORE);
    _ignoredFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_renamedFileItem);

    strings.clear();
    strings.append(tr("Errors"));
    _errorFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_ERROR );
    _errorFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_errorFileItem);

    strings.clear();
    strings.append(tr("Conflicts"));
    _conflictFileItem = new QTreeWidgetItem( _ui->_treeWidget, strings, TYPE_CONFLICT);
    _conflictFileItem->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    _ui->_treeWidget->addTopLevelItem(_conflictFileItem);

    QList<QTreeWidgetItem*> syncedItems;
    QList<QTreeWidgetItem*> renamedItems;
    QList<QTreeWidgetItem*> newItems;
    QList<QTreeWidgetItem*> deletedItems;
    QList<QTreeWidgetItem*> ignoredItems;
    QList<QTreeWidgetItem*> conflictItems;
    QList<QTreeWidgetItem*> errorItems;

    quint64 overall_files = 0;

    foreach( SyncFileItem item, list ) {
        overall_files++;

        QString dir;
        QStringList str( item._file );
        if( item._dir == SyncFileItem::Up )   dir = tr("Uploaded");
        if( item._dir == SyncFileItem::Down ) dir = tr("Downloaded");
        str << dir;

        switch( item._instruction ) {
        case   CSYNC_INSTRUCTION_NONE:
            // do nothing.
            break;
        case CSYNC_INSTRUCTION_EVAL:
            // should not happen
            break;
        case CSYNC_INSTRUCTION_REMOVE:
        case CSYNC_INSTRUCTION_DELETED:
            deletedItems.append( new QTreeWidgetItem(_deletedFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_RENAME:
            renamedItems.append( new QTreeWidgetItem(_renamedFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_NEW:
            newItems.append( new QTreeWidgetItem(_newFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_CONFLICT:
            conflictItems.append( new QTreeWidgetItem(_conflictFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_IGNORE:
            ignoredItems.append( new QTreeWidgetItem(_ignoredFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_SYNC:
        case CSYNC_INSTRUCTION_UPDATED:
            syncedItems.append( new QTreeWidgetItem(_syncedFileItem, str, FILE_TYPE) );
            break;
        case CSYNC_INSTRUCTION_STAT_ERROR:
        case CSYNC_INSTRUCTION_ERROR:
            errorItems.append( new QTreeWidgetItem(_errorFileItem, str, FILE_TYPE) );
            break;
        default:
            break;
        }
    }

    formatHeaderItem( _syncedFileItem, syncedItems );
    formatHeaderItem( _newFileItem, newItems );
    formatHeaderItem( _deletedFileItem, deletedItems );
    formatHeaderItem( _renamedFileItem, renamedItems );
    formatHeaderItem( _errorFileItem, errorItems );
    formatHeaderItem( _conflictFileItem, conflictItems );
    formatHeaderItem( _ignoredFileItem, ignoredItems );

}

void FileItemDialog::formatHeaderItem( QTreeWidgetItem *header, const QList<QTreeWidgetItem*>& list )
{
    if( !header ) return;

    header->addChildren( list );
    int count = list.count();
#if LEAVE_THAT_TO_DESIGNERS
    QColor col("#adc5d3");
    header->setBackgroundColor(0, col);
    header->setBackgroundColor(1, col);
#endif
    header->setText(1, QString::number( count ));
    if( count ) {
        QFont font;
        font.setWeight( QFont::Bold );
        header->setFont(0, font);
        header->setFont(1, font);
        header->setExpanded(true);
    } else {
        header->setExpanded(false);
    }
}

}
