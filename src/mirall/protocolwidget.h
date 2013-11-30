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

#ifndef PROTOCOLWIDGET_H
#define PROTOCOLWIDGET_H

#include <QDialog>
#include <QDateTime>
#include <QLocale>

#include "mirall/progressdispatcher.h"

#include "ui_protocolwidget.h"

namespace Mirall {
class SyncResult;

namespace Ui {
  class ProtocolWidget;
}
class Application;

class ProtocolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProtocolWidget(QWidget *parent = 0);
    ~ProtocolWidget();

    void setupList();

signals:

public slots:
    void slotProgressInfo( const QString& folder, const Progress::Info& progress );
    void slotProgressProblem(const QString& folder, const Progress::SyncProblem& problem );
    void slotOpenFile( QTreeWidgetItem* item, int );

protected slots:
    void copyToClipboard();

signals:
    void guiLog(const QString&, const QString&);

private:
    void setSyncResultStatus(const SyncResult& result );
    void cleanErrors( const QString& folder );
    QTreeWidgetItem* createProgressTreewidgetItem(const Progress::Info& progress );
    QTreeWidgetItem* createProblemTreewidgetItem( const Progress::SyncProblem& problem);

    QString timeString(QDateTime dt, QLocale::FormatType format = QLocale::NarrowFormat) const;

    const int ErrorIndicatorRole;
    Ui::ProtocolWidget *_ui;
    int _problemCounter;
};

}
#endif // PROTOCOLWIDGET_H
