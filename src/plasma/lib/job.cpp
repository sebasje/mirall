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


#include "job.h"

#include <QDebug>

class JobPrivate {
public:
    Job *q;
    int advance;
};

Job::Job(QObject* parent) :
    QObject(parent)
{
    d = new JobPrivate;
    d->q = this;
    d->advance = 0;
}

Job::~Job()
{
    delete d;
}


void Job::init()
{
}

void Job::start()
{
    setAdvance(0);
}

void Job::stop()
{
}

int Job::advance()
{
    return d->advance;
}

void Job::setAdvance(const int advance)
{
    if (d->advance != advance) {
        d->advance = advance;
        emit advanceChanged();
    }
}


#include "job.moc"
