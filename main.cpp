#include "mainwindow.h"

#include <QApplication>

#include "constant.h"
//#include "loginfo.h"

int main(int argc, char *argv[])
{
    //LogInfo::RecordLog();
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();
    return a.exec();
}
