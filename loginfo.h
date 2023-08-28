#ifndef LOGINFO_H
#define LOGINFO_H

#include <QtGlobal>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QTime>
#include <qt_windows.h>

class LogInfo
{
public:
    LogInfo();
    static void RecordLog();

private:
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    static void logThread::outputLog(QtMsgType type, const char* msg);
#else
    static void outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg);
#endif
};

#endif // LOGINFO_H
