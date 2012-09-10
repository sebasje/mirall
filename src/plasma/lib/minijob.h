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


#ifndef MINIJOB_H
#define MINIJOB_H

#include "job.h"
#include <QObject>

class MiniJobPrivate;

class MiniJob : public Job
{
    Q_OBJECT

    public:
        MiniJob(QObject* parent = 0);
        ~MiniJob();


    public Q_SLOTS:
        void init();

        Q_INVOKABLE virtual void start();

    private Q_SLOTS:
        void timeout();

    private:
        MiniJobPrivate* d;
};

#endif // MINIJOB_H
