#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
#include <QMessageBox>
#include <QDateTime>

class Config;
class DataTable;
class QTextBrowser;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Config* config = nullptr;
    DataTable* dataTable = nullptr;

    QString inputFilePath;
    QString updateSheetName;

    QMenuBar* menuBar;
    QMenu* menuHelp;
    QAction* actionAbout;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_importBookButton_clicked();
    void on_updateBookButton_clicked();
    void on_exportBookButton_clicked();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

private:
    void append_updateProcessTextBrowser(const QString& text);
    void updateTextBrowserContent(const QString& text, QTextBrowser* qtb);
    void appendTextBrowserContent(const QString& newText, QTextBrowser* qtb);

};
#endif // MAINWINDOW_H
