#include "mainwindow.h"

#include <QApplication>

#include "constant.h"

int main(int argc, char *argv[])
{
    constructMapFieldType();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
