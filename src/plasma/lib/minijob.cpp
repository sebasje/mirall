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


#include "minijob.h"

#include <QTimer>

#include <QDebug>

class MiniJobPrivate {
public:
    MiniJob *q;
    QTimer *t;
    int c;
};

MiniJob::MiniJob(QObject* parent) :
    Job(parent)
{
    qDebug() << "Creating Minijob";
    d = new MiniJobPrivate;
    d->t = 0;
    init();
}

MiniJob::~MiniJob()
{
    qDebug() << "doei!";
    delete d;
}

void MiniJob::init()
{
    d->c = 0;
    if (!d->t) {
        d->t = new QTimer(this);
    }
    connect(d->t, SIGNAL(timeout()), SLOT(timeout()));
    d->t->setInterval(100);
}

void MiniJob::timeout()
{
    //qDebug() << "Timeout";
    d->c = d->c + 4;
    setAdvance(d->c);
    if (d->c >= 100) {
        d->t->stop();
        emit finished(true);
        delete d->t;
        d->c = 100;
        deleteLater();
    }
}

void MiniJob::start()
{
    qDebug() << "Starting Minijob";
    d->t->start();
    Job::start();
    if (d->c >= 100) {
        d->c = 0;
        setAdvance(0);
    }
}

void MiniJob::stop()
{
    if (d->t) {
        d->t->stop();
    }
    qDebug();
    Job::stop();
    delete(d->t);
    d->t = 0;
    deleteLater();
}

#include "minijob.moc"
