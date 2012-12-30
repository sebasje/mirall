/*
 * Copyright (C) by Daniel Molkentin <danimo@owncloud.com>
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

#ifndef MIRALL_FOLDERWATCHER_INOTIFY_H
#define MIRALL_FOLDERWATCHER_INOTIFY_H

#include <QObject>

namespace Mirall {

class INotify;
class FolderWatcher;

class FolderWatcherPrivate : public QObject {
    Q_OBJECT
public:
    FolderWatcherPrivate(FolderWatcher *p);
private slots:
    void slotAddFolderRecursive(const QString &path);
    void slotINotifyEvent(int mask, int cookie, const QString &path);
private:
    INotify *_inotify;
    FolderWatcher *_parent;
    // to cancel events that belong to the same action
    int _lastMask;
    QString _lastPath;
};

}

#endif // MIRALL_FOLDERWATCHER_INOTIFY_H
