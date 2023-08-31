#include "loginfo.h"

LogInfo::LogInfo()
{}

void LogInfo::RecordLog()
{
    //判断当前是否处于调试环境，如果有调试器，就不会把日志信息输出到日志文件
    if (IsDebuggerPresent()) {
        return;
    }
    //安装消息处理函数，传入回调函数，输出调试信息时调用
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    logThread::qInstallMsgHandler(outputLog);
#else
    qInstallMessageHandler(outputLog);
#endif
}


void LogInfo::outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QDir projectDir(QDir::currentPath()); //当前工程目录下
    const QString logDir = "log";
    if (!projectDir.exists(logDir)) {
        projectDir.mkdir(logDir);
    }
    projectDir.cd(logDir); //切换到日志目录下
    QString logPath = projectDir.filePath(QDate::currentDate().toString("yyyy-MM-dd") + ".txt");

    QFile logFile(logPath);
    if (!logFile.open(QFile::Append | QFile::WriteOnly)) {
        qInstallMessageHandler(0); //不再输出到日志文件
        qWarning("ERROR OPEN LOG FILE");
        return;
    }

    static QMutex mutex;
    mutex.lock();
    QString typeStr = nullptr;
    switch(type) {
    case QtDebugMsg:    typeStr = "Debug"; break;
    case QtInfoMsg:     typeStr = "Info"; break;
    case QtWarningMsg:  typeStr = "Warning"; break;
    case QtCriticalMsg: typeStr = "Critical"; break;
    case QtFatalMsg:    typeStr = "Fatal"; break;
    default:            break;
    }
    QString logLine = QString("%1%2    %3:%4:%5    %7%8\r\n").arg("Time::")
            .arg(QTime::currentTime().toString("hh:mm:ss")).arg(typeStr)
            .arg(context.file).arg(context.line).arg(context.function)
            .arg("Log::").arg(msg);
    logFile.write(logLine.toUtf8());
    logFile.close();
    mutex.unlock();
    //OutputDebugStringA(msg.toLocal8Bit());
}





