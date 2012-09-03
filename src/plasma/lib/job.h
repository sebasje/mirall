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


#ifndef JOB_H
#define JOB_H

#include <QObject>

class JobPrivate;

class Job : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int advance READ advance NOTIFY advanceChanged)

    public:
        Job(QObject* parent = 0);
        ~Job();


    public Q_SLOTS:
        virtual void init();

        int advance();
        void setAdvance(const int advance);

        Q_INVOKABLE virtual void start();
        Q_INVOKABLE virtual void stop();

    Q_SIGNALS:
        void finished(bool success);
        void advanceChanged();

    private:
        JobPrivate* d;
};

#endif // JOB_H
