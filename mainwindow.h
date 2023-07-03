#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>

class Config;
class DataTable;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Config* config;
    DataTable* dataTable;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_importBookButton_clicked();
    void on_updateBookButton_clicked();
    void on_exportBookButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    void update_bookFilePathNameTextBrowser(const QString& text);
    void update_bookNameTextBrowser(const QString& text);

};
#endif // MAINWINDOW_H
