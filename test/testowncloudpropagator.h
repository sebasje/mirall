/*
   This software is in the public domain, furnished "as is", without technical
   support, and with no warranty, express or implied, as to its usefulness for
   any purpose.
*/

#ifndef MIRALL_TESTOWNCLOUDPROPAGATOR_H
#define MIRALL_TESTOWNCLOUDPROPAGATOR_H

#include <QtTest>
#include <QDebug>

#include "propagatedownload.h"

using namespace OCC;
namespace OCC {
QString OWNCLOUDSYNC_EXPORT createDownloadTmpFileName(const QString &previous);
}

class TestOwncloudPropagator : public QObject
{
    Q_OBJECT

private slots:
    void testUpdateErrorFromSession()
    {
//        OwncloudPropagator propagator( NULL, QLatin1String("test1"), QLatin1String("test2"), new ProgressDatabase);
        QVERIFY( true );
    }

    void testTmpDownloadFileNameGeneration()
    {
        QString fn;
        // without dir
        for (int i = 1; i <= 1000; i++) {
            fn+="F";
            QString tmpFileName = createDownloadTmpFileName(fn);
            if (tmpFileName.contains('/')) {
                tmpFileName = tmpFileName.mid(tmpFileName.lastIndexOf('/')+1);
            }
            QVERIFY( tmpFileName.length() > 0);
            QVERIFY( tmpFileName.length() <= 254);
        }
        // with absolute dir
        fn = "/Users/guruz/ownCloud/rocks/GPL";
        for (int i = 1; i < 1000; i++) {
            fn+="F";
            QString tmpFileName = createDownloadTmpFileName(fn);
            if (tmpFileName.contains('/')) {
                tmpFileName = tmpFileName.mid(tmpFileName.lastIndexOf('/')+1);
            }
            QVERIFY( tmpFileName.length() > 0);
            QVERIFY( tmpFileName.length() <= 254);
        }
        // with relative dir
        fn = "rocks/GPL";
        for (int i = 1; i < 1000; i++) {
            fn+="F";
            QString tmpFileName = createDownloadTmpFileName(fn);
            if (tmpFileName.contains('/')) {
                tmpFileName = tmpFileName.mid(tmpFileName.lastIndexOf('/')+1);
            }
            QVERIFY( tmpFileName.length() > 0);
            QVERIFY( tmpFileName.length() <= 254);
        }
    }
};

#endif
