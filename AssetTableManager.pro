QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    datatable.cpp \
    exportbookdialog.cpp \
    importbookdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    config.h \
    constant.h \
    datatable.h \
    exportbookdialog.h \
    importbookdialog.h \
    mainwindow.h

FORMS += \
    exportbookdialog.ui \
    importbookdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


QXLSX_PARENTPATH=../../Qt/QXlsx/QXlsx/          # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=../../Qt/QXlsx/QXlsx/header/   # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=../../Qt/QXlsx/QXlsx/source/   # current QXlsx source path is ./source/
include(../../Qt/QXlsx/QXlsx/QXlsx.pri)
